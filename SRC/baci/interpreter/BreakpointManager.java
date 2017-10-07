package baci.interpreter;

import baci.program.*;
import baci.event.*;
import java.util.*;
/**
 * Keeps track of current break points
 * @author: David Strite
 */
public class BreakpointManager {
	//Current program
	private Program program;
	//Current break points
	private HashMap breakpoints;
	//Listeners for break point changes
	private ArrayList listeners;
/**
 * Constructs a new BreakpointManager
 * @param p Program
 */
public BreakpointManager(Program p) {
	program = p;
	breakpoints = new HashMap();
	listeners = new ArrayList();
}
/**
 * Creates a new pcode break point at specified index
 * @param pcodeIndex
 */
public void addBreakpoint(int pcodeIndex) {
	if(!isPcodeBreakpoint(pcodeIndex)) {
		addBreakpoint(new Breakpoint(pcodeIndex,Breakpoint.TYPE_PCODE));
	}
}
/**
 * Adds a new source breakpoint at specified file and line
 * @param fileIndex int
 * @param lineIndex int
 */
public void addBreakpoint(int fileIndex, int lineIndex) {
	if(!isSourceBreakpoint(fileIndex,lineIndex)) {
		int pcodeIndex = program.getMappingFirst(fileIndex,lineIndex);
		if(pcodeIndex!=-1) {
			addBreakpoint(new Breakpoint(pcodeIndex,Breakpoint.TYPE_SOURCE));
		}
	}
}
/**
 * Adds a break point to the list of current break points
 */
private void addBreakpoint(Breakpoint breakpoint) {
	breakpoints.put(new Integer(breakpoint.getPcodeIndex()),breakpoint);
	fireBreakpointChange(breakpoint);
}
/**
 * Adds a listener to the list that's notified when break points change
 * @param l listener
 */
public void addBreakpointlListener(BreakpointListener l) {
	listeners.add(l);
}
/**
 * Adds a temporary breakpoint at the specified pcode address for the specified process
 * @param pcodeIndex
 * @param process
 */
public void addTempBreakpoint(int pcodeIndex, BaciProcess process) {
	if(!isBreakpoint(pcodeIndex,null)) {
		addBreakpoint(new Breakpoint(pcodeIndex,Breakpoint.TYPE_TEMP,process.getIndex()));
	}
}
/**
 * Notifies listeners when a breakpoint changes
 * @param breakpoint
 */
private void fireBreakpointChange(Breakpoint breakpoint) {
	for(int i=listeners.size()-1; i>=0; i--) {
		((BreakpointListener)listeners.get(i)).breakpointChanged(new BreakpointChangeEvent(this,breakpoint.getPcodeIndex(),breakpoint.getType()));
	}
}
/**
 * Determines if there is a break point at the specified pcode address applying to the specified process.
 * process of null can be specified to check for any process
 * @param pcodeIndex
 * @param process
 * @return boolean
 */
public boolean isBreakpoint(int pcodeIndex, BaciProcess process) {
	Breakpoint breakpoint = (Breakpoint)breakpoints.get(new Integer(pcodeIndex));
	if(breakpoint!=null) {
		if(breakpoint.getType()==Breakpoint.TYPE_TEMP) {
			if(process!=null && process.getIndex()==breakpoint.getProcessIndex()) {
				removeBreakpoint(breakpoint.getPcodeIndex());
				return true;
			} else {
				return false;
			}
		}
		return true;
	}
	return false;
}
/**
 * Determines if there is a pcode breakpoint at the specified address
 * @param pcodeIndex
 * @return boolean
 */
public boolean isPcodeBreakpoint(int pcodeIndex) {
	Object breakpoint = breakpoints.get(new Integer(pcodeIndex));
	if(breakpoint!=null && ((Breakpoint)breakpoint).getType()==Breakpoint.TYPE_PCODE) {
		return true;
	}
	return false;
}
/**
 * Determines if there is a source break point at the specified file and line
 * @param fileIndex
 * @param lineIndex
 * @return boolean
 */
public boolean isSourceBreakpoint(int fileIndex, int lineIndex) {
	int pcodeIndex = program.getMappingFirst(fileIndex,lineIndex);
	Object breakpoint = breakpoints.get(new Integer(pcodeIndex));
	if(breakpoint!=null && ((Breakpoint)breakpoint).getType()==Breakpoint.TYPE_SOURCE) {
		return true;
	}
	return false;
}
/**
 * Remove a breakpoint from specified pcode address
 * @param pcodeIndex
 */
public void removeBreakpoint(int pcodeIndex) {
	Breakpoint breakpoint = (Breakpoint)breakpoints.remove(new Integer(pcodeIndex));
	if(breakpoint!=null) {
		fireBreakpointChange(breakpoint);
	}
}
/**
 * Removes a break point from file and line
 * @param fileIndex
 * @param lineIndex
 */
public void removeBreakpoint(int fileIndex, int lineIndex) {
	int pcodeIndex = program.getMappingFirst(fileIndex,lineIndex);
	Breakpoint breakpoint = (Breakpoint)breakpoints.remove(new Integer(pcodeIndex));
	if(breakpoint!=null) {
		fireBreakpointChange(breakpoint);
	}
}
/**
 * Remove a listener from the list that's notified when a break point changes
 * @param l listener
 */
public void removeBreakpointListener(BreakpointListener l) {
	listeners.remove(l);
}
}
