package baci.gui;

import baci.event.*;
import java.util.ArrayList;
import baci.interpreter.*;
import javax.swing.event.*;
import javax.swing.table.*;
/**
 * Table model for process data
 * @author: Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class ProcessTableModel implements TableModel, ProcessListener {
    private ArrayList listeners;
    private Interpreter interpreter;
/**
 * Constructs new ProcessTableModel
 * @param process
 */
public ProcessTableModel(Interpreter i) {
    interpreter = i;
//  addListener(this);
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
        case 0: return String.class;
        case 1: return Boolean.class;
        case 2: return Address.class;
        case 3: return Boolean.class;
        case 4: return Address.class;
        case 5: return Integer.class;
        case 6: return Integer.class;
    }
    return null;
}
/**
 * Gets the number of columns in the table
 * @return int
 */
public int getColumnCount() {
    return 7;
}
/**
 * Gets the name of the specified column
 * @param columnIndex
 * @return String
 */
public String getColumnName(int columnIndex) {
    switch(columnIndex) {
      case 0: return Config.PROC_PROCESS;
      case 1: return Config.PROC_ACTIVE;
      case 2: return Config.PROC_SUSPEND;
      case 3: return Config.PROC_FINISH;
      case 4: return Config.PROC_MONITOR;
      case 5: return Config.PROC_PRIORITY;
      case 6: return Config.PROC_ATOMIC;
    }
    return null;
}
/**
 * Gets the process associated with this StackTableModel
 * @return BaciProcess
 */
/*
public BaciProcess getProcess() {
    return process;
}
*/
/**
 * Gets the number of rows in the table
 * @return int
 */
public int getRowCount() {
    return interpreter.getProcesses().getSize();
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
        case 0:  return new Integer(rowIndex);
        default: return new Integer(0);
    }
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
        fireTableDataChanged();
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
