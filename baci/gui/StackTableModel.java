package baci.gui;

import baci.event.*;
import java.util.ArrayList;
import baci.interpreter.*;
import javax.swing.event.*;
import javax.swing.table.*;
/**
 * Table model for stack data
 * @author: David Strite
 */
public class StackTableModel implements TableModel, StackListener, ProcessListener {
    private ArrayList listeners;
    private baci.interpreter.BaciProcess process;
/**
 * Constructs new StackTableModel with specified process
 * @param process
 */
public StackTableModel(BaciProcess proc) {
    process = proc;
    process.addListener(this);
    process.getStack().addStackListener(this);
    listeners = new ArrayList();
}
/**
 * Add a listener to the list that's notified each time the table data changes
 * @param l listener
 */
public void addTableModelListener(TableModelListener l) {
    listeners.add(l);
}
/**
 * Notify listeners that the specified cell has been updated
 * @param row
 * @param column
 */
private void fireTableCellUpdated(int row, int column) {
    fireTableChanged(new TableModelEvent(this, row, row, column));
}
/**
 * Notify listeners that the table data has changed
 * @param e
 */
private void fireTableChanged(TableModelEvent e) {
    for(int i=listeners.size()-1; i>=0; i--) {
        ((TableModelListener)listeners.get(i)).tableChanged(e);
    }
}
/**
 * Notify listeners that table data has changed
 */
private void fireTableDataChanged() {
    fireTableChanged(new TableModelEvent(this));
}
/**
 * Gets the Class for the specified column
 * @param columnIndex
 * @return Class
 */
public Class getColumnClass(int columnIndex) {
    switch(columnIndex) {
        case 0:
            return Integer.class;
        case 1:
            return Object.class;
    }
    return null;
}
/**
 * Gets the number of columns in the table
 * @return int
 */
public int getColumnCount() {
    return 2;
}
/**
 * Gets the name of the specified column
 * @param columnIndex
 * @return String
 */
public String getColumnName(int columnIndex) {
    switch(columnIndex) {
        case 0:
            return Config.STACK_INDEX;
        case 1:
            return Config.STACK_VALUE;
    }
    return null;
}
/**
 * Gets the process associated with this StackTableModel
 * @return BaciProcess
 */
public BaciProcess getProcess() {
    return process;
}
/**
 * Gets the number of rows in the table
 * @return int
 */
public int getRowCount() {
    return process.getTop()+1;
}
/**
 * Gets the value for the specified cell
 *
 * @param rowIndex
 * @param columnIndex
 * @return Object
 */
public Object getValueAt(int rowIndex, int columnIndex) {
    switch(columnIndex) {
        case 0:
            return new Integer(rowIndex);
        case 1:
            return process.getStack().get(rowIndex);
    }
    return null;
}
/**
 * Determines if the specified cell is editable
 * @param rowIndex
 * @param columnIndex
 * @return boolean
 */
public boolean isCellEditable(int rowIndex, int columnIndex) {
    return false;
}
/**
 * Called when a process property changes
 * @param event
 */
public void processChanged(ProcessChangeEvent e) {
    if(e.getProperty()==ProcessChangeEvent.TOP) {
        fireTableDataChanged();
    }
}
/**
 * Removes a listener from the list that's notified when table data changes
 * @param l listener
 */
public void removeTableModelListener(TableModelListener l) {
    listeners.remove(l);
}
/**
 * Sets the value of the specified cell
 * @param aValue
 * @param rowIndex
 * @param columnIndex
 */
public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
    //not implemented because table is non-editable
}
/**
 * Called when the stack is changed
 * @param e StackChangeEvent
 */
public void stackChanged(StackChangeEvent e) {
    fireTableCellUpdated(e.getIndex(),1);
}
}
