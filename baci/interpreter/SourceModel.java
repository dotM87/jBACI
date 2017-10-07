package baci.interpreter;

import baci.event.*;
import baci.gui.Config;
import baci.program.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
/**
 * Table model for source data
 * @author: David Strite
 */
public class SourceModel extends CodeModel implements TableModel, CurrentPcodeListener, BreakpointListener {
	private BaciProcess process;
	//Index of file being displayed
	private int fileIndex;
	//Line index of current line
	private int fileLineIndex;
/**
 * Constructs a new SourceModel with specified process
 * @param proc
 */
public SourceModel(BaciProcess proc) {
	process = proc;
}
/**
 * Called when a break point changes
 * @param event
 */
public void breakpointChanged(BreakpointChangeEvent event) {
	int index = event.getPcodeIndex();
	PcodeInstruction pcode = process.getProgram().getPcode(index);
	char type = event.getType();
	if(fileIndex==pcode.getFileIndex() && type==Breakpoint.TYPE_SOURCE) {
		int lineIndex = pcode.getFileLineIndex();
		fireTableDataChanged();
	}
}
/**
 * Called when the current pcode changes
 * @param event
 */
public void currentPcodeChanged(CurrentPcodeChangeEvent event) {
	int newCurrent = event.getNewCurrentPcode();
	Program program = getProcess().getProgram();
	PcodeInstruction pcode = program.getPcode(newCurrent);

	if(pcode.getFileIndex()!=fileIndex) {
		fileIndex = pcode.getFileIndex();
	}
	fileLineIndex = pcode.getFileLineIndex();
	fireTableDataChanged();
}
/**
 * Gets the Class of specified column
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
 * Gets number of columns
 * @return int
 */
public int getColumnCount() {
	return 2;
}
/**
 * Gets the name for specified column
 * @param columnIndex
 * @return String
 */
public String getColumnName(int columnIndex) {
	switch(columnIndex) {
		case 0: {
			return Config.SOURCE_STATUS;
		}
		case 1: {
			return Config.SOURCE_CODE;
		}
	}
	return null;
}
/**
 * Gets the row index for the current line
 * @return int
 */
public int getCurrentRow() {
	return fileLineIndex;
}
/**
 * Gets the process associated with this SourceModel
 * @return BaciProcess
 */
public BaciProcess getProcess() {
	return process;
}
/**
 * Gets the number of row in the table
 * @return int
 */
public int getRowCount() {
	return process.getProgram().getFileSource(fileIndex).size();
}
/**
 * Gets the value of the specified cell
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
			boolean atCurrent = rowIndex==fileLineIndex;
			boolean atBreakPoint = process.getInterpreter().getBreakpointManager().isSourceBreakpoint(fileIndex,rowIndex);
			if(atCurrent && atBreakPoint) {
				//Current Breakpoint
				return "S";
			} if(rowIndex==fileLineIndex) {
				//Current
				return "C";
			} else if(process.getInterpreter().getBreakpointManager().isSourceBreakpoint(fileIndex,rowIndex)) {
				//Breakpoint
				return "B";
			}
			return "";
		}
		case 1: {
			return process.getProgram().getFileSource(fileIndex).get(rowIndex);
		}
	}
	return null;
}
/**
 * Determines if specified cell is editable
 * @param rowIndex
 * @param columnIndex
 * @return boolean
 */
public boolean isCellEditable(int rowIndex, int columnIndex) {
	return false;
}
/**
 * Sets the value of the specified cell
 * @param aValue
 * @param rowIndex
 * @param columnIndex
 */
public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
	//Not implemented because table is non editable
}
}
