import ctypes
from PyQt5 import Qt

class ListRoleModel(Qt.QAbstractListModel):
    def __init__(self, property_names=None, signaling_list=None, parent=None):
        super().__init__(parent)
        self._signaling_list = None
        self.property_names = property_names
        self.property_rolenames = {property_role : property_name for property_role, property_name in enumerate(property_names, Qt.Qt.UserRole + 1)}
        self.property_rolenames_utf8 = {property_role : property_name.encode('utf-8') for property_role, property_name in enumerate(property_names, Qt.Qt.UserRole + 1)}
        self.property_nameroles = {property_name : property_role for property_role, property_name in enumerate(property_names, Qt.Qt.UserRole + 1)}
        self._instance_counts = dict()
        self.signaling_list = signaling_list

    def roleNames(self):
        return self.property_rolenames_utf8

    def rowCount(self, _=None):
        sl = self.signaling_list
        return 0 if sl is None else len(sl)

    def flags(self, midx):
        f = Qt.Qt.ItemIsSelectable | Qt.Qt.ItemNeverHasChildren
        if midx.isValid():
            f |= Qt.Qt.ItemIsEnabled | Qt.Qt.ItemIsEditable
        f |= Qt.Qt.ItemIsDragEnabled if midx.isValid() else Qt.Qt.ItemIsDropEnabled
        return f

    def get_row_value_for_role(self, row, property_role):
        try:
            property_name = self.property_rolenames[property_role]
            return getattr(self.signaling_list[row], property_name, None)
        except:
            return

    def data(self, midx, role=Qt.Qt.DisplayRole):
        if midx.isValid():
            return Qt.QVariant(self.get_row_value_for_role(midx.row(), role))
        return Qt.QVariant()

    def set_row_value_for_role(self, row, property_role, v):
        try:
            property_name = self.property_rolenames[property_role]
            setattr(self.signaling_list[row], property_name, v)
        except:
            return

    def setData(self, midx, value, role=Qt.Qt.EditRole):
        if midx.isValid():
            if isinstance(value, Qt.QVariant):
                value = value.value()
            self.set_row_value_for_role(midx.row(), role, value)
            return True
        return False

#   def headerData(self, section, orientation, role=Qt.Qt.DisplayRole):
#       if orientation == Qt.Qt.Vertical:
#           if role == Qt.Qt.DisplayRole and 0 <= section < self.rowCount():
#               return Qt.QVariant(section)
#       elif orientation == Qt.Qt.Horizontal:
#           if role == Qt.Qt.DisplayRole and 0 <= section < self.columnCount():
#               return Qt.QVariant(self.property_names[section])
#       return Qt.QVariant()

    def removeRows(self, row, count, parent=Qt.QModelIndex()):
#       print('removeRows', row, count, parent)
        try:
            del self.signaling_list[row:row+count]
            return True
        except IndexError:
            return False

    @property
    def signaling_list(self):
        return self._signaling_list

    @signaling_list.setter
    def signaling_list(self, v):
        if v is not self._signaling_list:
            if self._signaling_list is not None or v is not None:
                self.beginResetModel()
                try:
                    if self._signaling_list is not None:
                        self._signaling_list.inserting.disconnect(self._on_inserting)
                        self._signaling_list.inserted.disconnect(self._on_inserted)
                        self._signaling_list.replaced.disconnect(self._on_replaced)
                        self._signaling_list.removing.disconnect(self._on_removing)
                        self._signaling_list.removed.disconnect(self._on_removed)
                        self._detach_elements(self._signaling_list)
                    self._signaling_list = v
                    if v is not None:
                        v.inserting.connect(self._on_inserting)
                        v.inserted.connect(self._on_inserted)
                        v.replaced.connect(self._on_replaced)
                        v.removing.connect(self._on_removing)
                        v.removed.connect(self._on_removed)
                        self._attach_elements(v)
                finally:
                    self.endResetModel()

    def _attach_elements(self, elements):
        property_names = self.property_names
        for element in elements:
            instance_count = self._instance_counts.get(element, 0) + 1
            assert instance_count > 0
            self._instance_counts[element] = instance_count
            if instance_count == 1:
                for property_name in self.property_names:
                    property_role = self.property_nameroles[property_name]
                    try:
                        l = lambda element, property_role=property_role: self._on_property_changed(element, property_role)
                        setattr(element, '_{}LAMBDA_'.format(property_name), l)
                        getattr(element, property_name + 'Changed').connect(l)
                    except AttributeError:
                        pass

    def _detach_elements(self, elements):
        for element in elements:
            instance_count = self._instance_counts[element] - 1
            assert instance_count >= 0
            if instance_count == 0:
                for property_name in self.property_names:
                    property_role = self.property_nameroles[property_name]
                    try:
                        getattr(element, property_name + 'Changed').disconnect(getattr(element, '_{}LAMBDA_'.format(property_name)))
                    except AttributeError:
                        pass
                del self._instance_counts[element]
            else:
                self._instance_counts[element] = instance_count

    def _on_property_changed(self, element, property_role):
        sl = self.signaling_list
        next_row = 0
        instance_count = self._instance_counts[element]
        assert instance_count > 0
        for _ in range(instance_count):
            row = sl.index(element, next_row)
            next_row = row + 1
            self.dataChanged.emit(self.createIndex(row, 0), self.createIndex(row, 0), [property_role])

    def _on_inserting(self, idx, elements):
        self.beginInsertRows(Qt.QModelIndex(), idx, idx+len(elements)-1)

    def _on_inserted(self, idx, elements):
        self.endInsertRows()
        self._attach_elements(elements)

    def _on_replaced(self, idxs, replaced_elements, elements):
        self._detach_elements(replaced_elements)
        self._attach_elements(elements)
        self.dataChanged.emit(self.createIndex(min(idxs), 0), self.createIndex(max(idxs), 0))

    def _on_removing(self, idxs, elements):
        self.beginRemoveRows(Qt.QModelIndex(), min(idxs), max(idxs))

    def _on_removed(self, idxs, elements):
        self.endRemoveRows()
        self._detach_elements(elements) 
