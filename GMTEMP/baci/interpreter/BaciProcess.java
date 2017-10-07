package baci.interpreter;

import baci.event.*;
import baci.gui.*;
import baci.program.*;
import java.util.*;
/**
 * Represents a process running in the interpretter
 * @author: David Strite
 */

public class BaciProcess implements StackListener {
    //name of the process, usually the identifier of the function
    //or process called from cobegin block
    private String name;
    //index of this process in the process table
    private int index;
    //index of current pcode instruction for this process
    private int currentPcode;
    //program this process is running
    private Program program;
    //interpreter this process is running in
    private Interpreter interpreter;
    //current stack frame
    private int top;
    private int bottom;
    //current display
    private Stack display;
    //identifier index for this process
    private int identifierIndex;
    //is process currently running? 
    private boolean active;
    //address of object this process is suspended on
    private Address suspend;
    //address of a monitor this process is currently in
    private Address monitor;
    private String monitorName;
    //process priority
    private int priority;
    //number of atomic calls
    private int atomicCount;
    //has process finished running?
    private boolean finished;
    //stack for this process
    private Stack stack;
    //table models for source and pcode
    private SourceModel sourceModel;
    private PcodeModel pcodeModel;
    //table model for variable data
    private VariableModel variableModel;
    //console to write output to from this process
    private Console console;
    //listeners for process change events
    private ArrayList listeners;
/**
 * Creates a new BaciProcess with specified parameters
 * @param name of this process
 * @param index in process table for this process
 * @param prog program this process will run
 * @param interpreter this process will run in
 */
public BaciProcess(String name, int index, Program prog, Interpreter interpreter) {
    this.name = index + " " + name;
    this.index = index;
    this.interpreter = interpreter;
    program = prog;

    sourceModel = new SourceModel(this);
    pcodeModel = new PcodeModel(this);
    variableModel = new VariableModel(this);

    console = new Console();
    stack = new Stack();
    stack.addStackListener(this);

    display = new Stack();
    listeners = new ArrayList();
    
    ProcessTableModel p = interpreter.getDebugger().getDebuggerFrame().getProcessTableModel();
    listeners.add(p);
}
/**
 * Adds a listener that is notified when a property of this process changes
 *
 * @param l ProcessListener
 */
public void addListener(ProcessListener l) {
    listeners.add(l);
}
/**
 * Determines if this process is equal to another
 * @return boolean
 * @param o 
 */
public boolean equals(Object o) {
    if(o instanceof BaciProcess) {
        BaciProcess p = (BaciProcess)o;
        return index==p.index;
    }
    return false;
}
/**
 * Notify listeners that property has changed
 * @param propery that has changed
 * @param oldValue
 * @param newValue
 */
private void firePropertyChanged(int property, Object oldValue, Object newValue) {
    ProcessChangeEvent e = new ProcessChangeEvent(this,property,oldValue,newValue);
    for(int i=listeners.size()-1; i>=0; i--) {
        ((ProcessListener)listeners.get(i)).processChanged(e);
    }
}
/**
 * Gets the number of atomic calls that have been made
 * @return int
 */
public int getAtomicCount() {
    return atomicCount;
}
/**
 * Gets the bottom of the stack frame
 * @return int
 */
public int getBottom() {
    return bottom;
}
/**
 * Gets the console used by this process
 * @return Console
 */
public Console getConsole() {
    return console;
}
/**
 * Gets the current pcode address for this process
 * @return int
 */
public int getCurrentPcode() {
    return currentPcode;
}
/**
 * Gets the display for this process
 * @return Stack
 */
public Stack getDisplay() {
    return display;
}
/**
 * Gets the identifier index for this process
 * @return int
 */
public int getIdentifierIndex() {
    return identifierIndex;
}
/**
 * Gets the index in the process table for this process
 * @return int
 */
public int getIndex() {
    return index;
}
/**
 * Gets the interpreter running this process
 * @return Interpreter
 */
public Interpreter getInterpreter() {
    return interpreter;
}
/**
 * Gets the address of a monitor this process is in
 * @return Address
 */
public Address getMonitor() {
    return monitor;
}
public String getMonitorName() {
    return monitorName;
}
/**
 * Gets the name of this process
 * @return String
 */
public java.lang.String getName() {
    return name;
}
/**
 * Gets the table model used for pcode data
 * @return PcodeModel
 */
public PcodeModel getPcodeModel() {
    return pcodeModel;
}
/**
 * Gets the priority for this process
 * @return int
 */
public int getPriority() {
    return priority;
}
/**
 * Gets the Program this process is running
 * @return baci.program.Program
 */
public Program getProgram() {
    return program;
}
/**
 * Gets the table model used for source data
 * @return SourceModel
 */
public SourceModel getSourceModel() {
    return sourceModel;
}
/**
 * Gets the stack for this process
 * @return Stack
 */
public Stack getStack() {
    return stack;
}
/**
 * Gets the address of an object this process is suspended on
 * @return Address
 */
public Address getSuspend() {
    return suspend;
}
/**
 * Gets the top of the stack frame
 * @return int
 */
public int getTop() {
    return top;
}
/**
 * Gets the table model for variable data
 * @return VariablesModel
 */
public VariableModel getVariableModel() {
    return variableModel;
}
/**
 * Determines if this process is running
 * @return boolean
 */
public boolean isActive() {
    return active;
}
/**
 * Determines if this process has finished running
 * @return boolean
 */
public boolean isFinished() {
    return finished;
}
/**
 * Removes a listener from the list that's notified when a property of this process is modified
 * @param   l listener
 */
public void removeListener(ProcessListener l) {
    listeners.remove(l);
}
/**
 * Sets if this process is currently running
 * @param newActive
 */
public void setActive(boolean newActive) {
    boolean oldActive = active;
    active = newActive;
    if(oldActive!=newActive) {
        firePropertyChanged(ProcessChangeEvent.ACTIVE,new Boolean(oldActive),new Boolean(newActive));
    }
    if ((index == 0) && (!newActive))  // CoEnd statement, show all windows if requested
        for (int i = 1; i < interpreter.getProcesses().getSize(); i++)
            interpreter.getDebugger().getDebuggerFrame().showActive(i);
}
/**
 * Sets the number of atomic calls that have been made
 * @param newAtomicCount
 */
public void setAtomicCount(int newAtomicCount) {
    int oldAtomicCount = atomicCount;
    atomicCount = newAtomicCount;
    if(oldAtomicCount!=newAtomicCount) {
        firePropertyChanged(ProcessChangeEvent.ATOMIC,new Integer(oldAtomicCount),new Integer(newAtomicCount));
    }
}
/**
 * Sets the bottom of the stack frame
 * @param newBottom 
 */
public void setBottom(int newBottom) {
    int oldBottom = bottom;
    bottom = newBottom;
    if(oldBottom!=newBottom) {
        firePropertyChanged(ProcessChangeEvent.BOTTOM,new Integer(oldBottom),new Integer(newBottom));
    }
}
/**
 * Sets the pcode address for the current instruction
 * @param newCurrentPcode
 */
public void setCurrentPcode(int newCurrentPcode) {
    int maxPcode = program.getPcodeSize()-1;
    if(newCurrentPcode>maxPcode) {
        newCurrentPcode = maxPcode;
    }
    int oldPcode = currentPcode;
    currentPcode = newCurrentPcode;
    CurrentPcodeChangeEvent event = new CurrentPcodeChangeEvent(this,oldPcode,currentPcode);
    pcodeModel.currentPcodeChanged(event);
    sourceModel.currentPcodeChanged(event);

    int currentBlock = program.getPcode(currentPcode).getBlockIndex();
}
/**
 * Sets if this process is finished running
 * @param newFinished
 */
public void setFinished(boolean newFinished) {
    boolean oldFinished = finished;
    finished = newFinished;
    if(oldFinished!=newFinished) {
        firePropertyChanged(ProcessChangeEvent.FINISHED,new Boolean(oldFinished),new Boolean(newFinished));
    }
}
/**
 * Sets the index of the identifier for this process
 * @param newIdentifierIndex
 */
public void setIdentifierIndex(int newIdentifierIndex) {
    identifierIndex = newIdentifierIndex;
}
/**
 * Sets the address of a monitor this process is in
 * @param newMonitor
 */
public void setMonitor(Address newMonitor, String name) {
    Address oldMonitor = monitor;
    monitor = newMonitor;
    monitorName = name;
    if(oldMonitor!=newMonitor) {
        firePropertyChanged(ProcessChangeEvent.MONITOR,oldMonitor,newMonitor);
    }
}
/**
 * Sets the priority for this process
 * @param newPriority
 */
public void setPriority(int newPriority) {
    int oldPriority = priority;
    priority = newPriority;
    if(oldPriority!=newPriority) {
        firePropertyChanged(ProcessChangeEvent.PRIORITY,new Integer(oldPriority),new Integer(newPriority));
    }
}
/**
 * Sets the address of an object this process is suspended on
 * @param newSuspend
 */
public void setSuspend(Address newSuspend) {
    Address oldSuspend = suspend;
    suspend = newSuspend;
    if(oldSuspend!=newSuspend) {
        firePropertyChanged(ProcessChangeEvent.SUSPEND,oldSuspend,newSuspend);
    }
}
/**
 * Sets the top of the stack frame
 * @param newTop
 */
public void setTop(int newTop) {
    int oldTop = top;
    top = newTop;
    if(oldTop!=newTop) {
        firePropertyChanged(ProcessChangeEvent.TOP,new Integer(oldTop),new Integer(newTop));
    }
}
/**
 * Called when a stack this process is listening to changes
 * @param event
 */
public void stackChanged(StackChangeEvent event) {
    int index = event.getIndex();
    VariableModel.VariableNode node = variableModel.getNodeForStackIndex(index);
    if(node!=null) {
        node.nodeChanged();
    }
}
/**
 * Gets the string representation of this process
 * @return String
 */
public String toString() {
    return name;
}
}
