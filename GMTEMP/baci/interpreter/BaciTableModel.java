package baci.interpreter;

import baci.program.*;
import java.awt.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
/**
 * Super class of table models used in debugger
 * @author: David Strite
 */
public abstract class BaciTableModel implements TableModel {
	//Listeners to this model
	private ArrayList listeners;
/**
 * Creates a new BaciTableModel
 */
public BaciTableModel() {
	listeners = new ArrayList();
}
/**
 * Add a listener to the list that's notified when the table data changes
 * @param l listener
 */
public void addTableModelListener(TableModelListener l) {
	listeners.add(l);
}
/**
 * Notifies listeners when a cell has been updated
 * @param row
 * @param column
 */
protected void fireTableCellUpdated(int row, int column) {
	fireTableChanged(new TableModelEvent(this, row, row, column));
}
/**
 * Notifies listeners of TableModelEvent
 * @param e TableModelEvent
 */
protected void fireTableChanged(TableModelEvent e) {
	for(int i=listeners.size()-1; i>=0; i--) {
		((TableModelListener)listeners.get(i)).tableChanged(e);
	}
}
/**
 * Notifies listeners when table data changes
 * @param row
 * @param column
 */
protected void fireTableDataChanged() {
	fireTableChanged(new TableModelEvent(this));
}
/**
 * Notifies listeners when rows have been updated
 * @param firstRow
 * @param lastRow
 */
protected void fireTableRowsUpdated(int firstRow, int lastRow) {
	fireTableChanged(new TableModelEvent(this, firstRow, lastRow, TableModelEvent.ALL_COLUMNS, TableModelEvent.UPDATE));
}
/**
 * Gets the width need for a column
 * @column
 * @metics FontMetrics
 * @return int
 */
public int getMaxWidth(int column, FontMetrics metrics) {
	int maxWidth = 0;
	int width;
	for(int i=getRowCount()-1; i>=0; i--) {
		width = metrics.stringWidth(getValueAt(i,column).toString());
		if(width>maxWidth) {
			maxWidth = width;
		}
	}
	return maxWidth;
}
/**
 * Remove a listener from the list that's notified when this table changes
 * @param l listener
 */
public void removeTableModelListener(TableModelListener l) {
	listeners.remove(l);
}
}
