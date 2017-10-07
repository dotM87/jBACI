package baci.interpreter;

import baci.event.*;
import baci.gui.Config;
import baci.program.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
/**
 * Table model for pcode data
 * @author: David Strite
 */
public class PcodeModel extends CodeModel implements TableModel, CurrentPcodeListener, BreakpointListener {
	private BaciProcess process;
	//Indexes of displayed pcode instructions
	private int beginIndex;
	private int endIndex;
/**
 * Constructs a new PcodeModel with specified process
 * @param proc
 */
public PcodeModel(BaciProcess proc) {
	process=proc;
	beginIndex=-1;
	endIndex=-1;
}
/**
 * Called when a break point has changed
 * @param event
 */
public void breakpointChanged(BreakpointChangeEvent event) {
	int index = event.getPcodeIndex();
	char type = event.getType();
	if(index>=beginIndex && index<=endIndex && type==Breakpoint.TYPE_PCODE) {
		fireTableDataChanged();
	}
}
/**
 * Called when current pcode address changes
 * @param event
 */
public void currentPcodeChanged(CurrentPcodeChangeEvent event) {
	int newCurrent = event.getNewCurrentPcode();
	if(newCurrent>endIndex || newCurrent<beginIndex) {
		Program program = getProcess().getProgram();
		PcodeInstruction pcode = program.getPcode(newCurrent);
		setRange(program.getMappingFirst(pcode.getFileIndex(),pcode.getFileLineIndex()),program.getMappingLast(pcode.getFileIndex(),pcode.getFileLineIndex()));
	} else {
		fireTableDataChanged();
	}
}
/**
 * Gets the first pcode index displayed
 * @return int
 */
public int getBeginIndex() {
	return beginIndex;
}
/**
 * Gets the column type for specified column
 * @param columnIndex
 * @return Class
 */
public Class getColumnClass(int columnIndex) {
	switch (columnIndex) {
		case 0 : {
			return String.class;
		}
		case 1 : {
			return String.class;
		}
	}
	return null;
}
/**
 * Gets the number of columns in table.
 * @return int
 */
public int getColumnCount() {
	return 2;
}
/**
 * Gets the name of specified column
 * @param columnIndex
 * @return String
 */
public String getColumnName(int columnIndex) {
	switch(columnIndex) {
		case 0: {
			return Config.PCODE_STATUS;
		}
		case 1: {
			return Config.PCODE_PCODE;
		}
	}
	return null;
}
/**
 * Gets the row for current instruction
 * @return int
 */
public int getCurrentRow() {
	return process.getCurrentPcode()-beginIndex;
}
/**
 * Gets the last pcode index displayed
 * @return int
 */
public int getEndIndex() {
	return endIndex;
}
/**
 * Gets the process associated with this PcodeModel
 * @return BaciProcess
 */
public BaciProcess getProcess() {
	return process;
}
/**
 * Gets the number of rows in table
 * @return int
 */
public int getRowCount() {
	if(beginIndex!=-1) {
		return endIndex - beginIndex +1;
	} else {
		return 0;
	}
}
/**
 * Gets the cell value at specified row,column
 * @param rowIndex
 * @param columnIndex
 * @return Object
 */
public Object getValueAt(int rowIndex, int columnIndex) {
	switch (columnIndex) {
		case 0: {
			if(process.isFinished()) {
				return "";
			}
			boolean atCurrent = rowIndex+beginIndex==process.getCurrentPcode();
			boolean atBreakpoint = process.getInterpreter().getBreakpointManager().isPcodeBreakpoint(rowIndex+beginIndex);
			if(atCurrent && atBreakpoint) {
				//Current at breakpoint
				return "S";
			} else if(atCurrent) {
				return "C";
			} else if(atBreakpoint) {
				return "B";
			}
			return "";
		}
		case 1: {
			return process.getProgram().getPcode(rowIndex+beginIndex);
		}
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
 * Sets the range of pcode instructions being displayed
 * @param begin int
 * @param end int
 */
public void setRange(int begin, int end) {
	beginIndex = begin;
	endIndex = end;
	fireTableDataChanged();
}
/**
 * Sets value of specified cell
 * @param aValue
 * @param rowIndex
 * @param columnIndex
 */
public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
	//Not implemented because table is non-editable
}
}
