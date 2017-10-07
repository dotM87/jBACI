package baci.interpreter;

import baci.graphics.*;
import baci.event.*;
import baci.exception.*;
import baci.gui.*;
import baci.program.*;
import java.util.*;
import javax.swing.table.*;
import java.io.*;

/**
 * Represents the BACI interpreter. It is responsible for storing information about current state of
 * interpreter and executing pcode instructions.
 * @author: David Strite
 * @author: Modified December 2002 by Moti Ben-Ari.
 * See copyright.txt.
 */
public class Interpreter implements StackListener, Runnable {
    //Program that is being run
    private Program program;
    //interpreter state
    private int status;
    //constants for states
    public static final int STATUS_RUN = 0;
    public static final int STATUS_FINISH = 1;
    public static final int STATUS_ERROR = 2;
    //number of instructions to execute before swap must occur
    private int numInstBeforeSwap;
    //maximum number of instructions allowed before swap
    public final static int MAX_BEFORE_SWITCH = 4;
    //index of current process
    private int currentProcessIndex;
    //is main thread currently in a cobegin/coend block?
    private boolean inCobeginBlock;
    //address for a short call
    private int shortCallAddress;
    //is the debugger running?
    private boolean running;
    //is a run being started?
    private boolean startingRun;
    //should interpreter stop on process swaps?
    private boolean stopOnSwap;
    //listeners to notify when the current process changes
    private ArrayList currentProcessListeners;
    //was the last character read a new line?
    private boolean readNewLine;
    //debugger  
    private Debugger debugger;
    //thread used when BACI program is running
    private Thread runThread;
    //List of current processes
    private ProcessList processes;
    //Common console
    private Console console;
    //table model for global variables
    private VariableModel globalVariableModel;
    //table model for history data
    private DefaultTableModel historyModel;
    //break point manager
    private BreakpointManager breakpointManager;
    //random number generator
    private Random randomGenerator;
    // List of graphics objects
    private GraphicsList graphicsList;
    // Source stepping or not
    private boolean sourceStepping;
    // History file writer
    private FileWriter out;
    // Linda tuple space
    private Linda linda;
    
/**
 * Constructs a new Interpreter
 * @param debugger
 */
public Interpreter(Debugger debugger) {
    processes = new ProcessList();
    randomGenerator = new Random();
    currentProcessListeners = new ArrayList();
    graphicsList = new GraphicsList();
    linda = new Linda();
    this.debugger = debugger;
}
/**
 * Add a listener to the list that's notified when the current process changes
 * @param l listener
 */
public void addCurrentProcessListener(CurrentProcessListener l) {
    currentProcessListeners.add(l);
}

/**
 * Change current process to that given by index.
 * Called when selecting from process table.
*/
public void changeToProcess(int index) {
    int oldProcessIndex = currentProcessIndex;
    currentProcessIndex = chooseFromStart(index, 1);
    fireCurrentProcessChange(new CurrentProcessChangeEvent(this,oldProcessIndex,currentProcessIndex));
}

/**
 * Changel current process number by delta +/- 1.
*/
public int nextProcess(int delta) {
    int oldProcessIndex = currentProcessIndex;
    int i = chooseProc(delta);
    if (i != -1) {
        currentProcessIndex = i;
        fireCurrentProcessChange(new CurrentProcessChangeEvent(this,oldProcessIndex,currentProcessIndex));
    }
    return i;
}

/**
 * Chooses a new process to run
 * @return int
 */
private int chooseProc(int delta) {
    //if main process is active, it should run
    if(processes.get(0).isActive()) {
        return 0;
    }
    int numProcs = processes.getSize();
    int current = delta == 0 ? randomGenerator.nextInt(numProcs) :
        ((currentProcessIndex + delta + numProcs) % numProcs);
    return chooseFromStart(current, delta);
}

/**
 * Choose a ready process, starting from current and searching
 * in the direction given by the sign of delta.
*/
private int chooseFromStart(int current, int delta) {
    int numProcs = processes.getSize();
    int count = 0;
    while((!processes.get(current).isActive() || processes.get(current).getSuspend()!=null) && count<numProcs) {
        if (delta < 0) current = (current-1+numProcs)%numProcs; else current = (current+1)%numProcs;
        count++;
    }
    if(count==numProcs) {
        return -1;
    }
    return current;
}

/**
 * Notify listeners that the current process has changed
 * @param event CurrentProcessChangeEvent
 */
private void fireCurrentProcessChange(CurrentProcessChangeEvent event) {
    for(int i=currentProcessListeners.size()-1; i>=0; i--) {
        ((CurrentProcessListener)currentProcessListeners.get(i)).currentProcessChanged(event);
    }
}
/**
 * Gets the BreakPointManager used by this Interpreter
 * @return BreakpointManager
 */
public BreakpointManager getBreakpointManager() {
    return breakpointManager;
}
/**
 * Gets the common console
 * @return Console
 */
public Console getConsole() {
    return console;
}

/**
 * Gets the index of the current process
 * @return int
*/
public int getCurrentProcessIndex() {
    return currentProcessIndex;
}

/**
 * Gets the process that is currently running
 * @return BaciProcess
 */
public BaciProcess getCurrentProcess() {
    if(inCobeginBlock) {
        return processes.get(processes.getSize()-1);
    } else {
        return processes.get(currentProcessIndex);
    }
}

/**
 * Gets the graphics list
 * @return GraphicsList
*/
public GraphicsList getGraphicsList() {
    return graphicsList;
}

/**
 * Gets the Linda tuple space
 * @return Linda
*/
public Linda getLinda() {
    return linda;
}

/**
 * Gets the Debugger associated with this Interpreter
 * @return Debugger
 */
public Debugger getDebugger() {
    return debugger;
}
/**
 * Gets the table model for global variables
 * @return VariablesModel
 */
public VariableModel getGlobalVariableModel() {
    return globalVariableModel;
}
/**
 * Gets the table model for history data
 * @return DefaultTableModel
 */
public DefaultTableModel getHistoryModel() {
    return historyModel;
}
/**
 * Gets the process at the specified indes
 * @param index
 * @return BaciProcess
 */
public BaciProcess getProcess(int index) {
    return processes.get(index);
}
/**
 * Gets the list of processes
 * @return ProcessList
 */
public ProcessList getProcesses() {
    return processes;
}
/**
 * Gets the program this Interpreter is running
 * @return Program
 */
public Program getProgram() {
    return program;
}
/**
 * Gets a random integer up to specified maximum
 * @param max
 * @return int
 */
public int getRandomInt(int max) {
    return randomGenerator.nextInt(max);
}
/**
 * Gets Thread used for run
 * @return Thread
 */
public Thread getRunThread() {
    return runThread;
}
/**
 * Gets pcode address for short call
 * @return int
 */
public int getShortCallAddress() {
    return shortCallAddress;
}
/**
 * Gets the status of this Interpreter
 * @return int
 */
public int getStatus() {
    return status;
}

public void setSourceStepping(boolean s) {
	sourceStepping = s;
}	

/**
 * Initializes this Interpreter to run specified program
 * @param p program to run
 */
public void initialize(Program p) {
    //create a thread for run
    runThread = new Thread(new RunRunnable(this),"Run Thread");
    runThread.setPriority(Thread.MIN_PRIORITY);
    runThread.start();
    
    program = p;
    processes.clear();

    breakpointManager = new BreakpointManager(p);

    console = new Console();

    currentProcessIndex=0;
    numInstBeforeSwap=0;
    inCobeginBlock=false;
    shortCallAddress=0;
    startingRun=false;
    sourceStepping = false;
    
    currentProcessListeners.clear();

    running=false;
    BaciProcess mainProc= new BaciProcess(p.getIdentifier(p.getBlock(0).getLastpar()).getName(),0,p,this);  
    processes.add(mainProc);

    Block block0 = program.getBlock(0);
    int mainBottom = block0.getVsize();
    mainProc.setBottom(mainBottom);
    int mainIdentifier = block0.getLastpar();
    mainProc.setIdentifierIndex(mainIdentifier);
    int mainTop = mainBottom+program.getBlock(program.getIdentifier(mainIdentifier).getRef()).getVsize()-1;
    mainProc.setTop(mainTop);
    Stack stack = mainProc.getStack();
    //Main activation record
    stack.set(mainBottom,0);
    stack.set(mainBottom+1,0);
    stack.set(mainBottom+2,new Address(mainProc,0));
    stack.set(mainBottom+3,new Address(mainProc,-1));
    stack.set(mainBottom+4,mainIdentifier);
    
    //Starting point of main
    mainProc.setCurrentPcode(program.getIdentifier(mainIdentifier).getAdr());
    mainProc.setActive(true);
    mainProc.setSuspend(null);
    
    //Initialize display
    Stack display = mainProc.getDisplay();
    display.set(0,new Address(mainProc,0));
    display.set(1,new Address(mainProc,mainBottom));
    p.buildVariables(mainProc.getVariableModel(),
        p.getPcode(mainProc.getCurrentPcode()).getBlockIndex(),mainProc.getBottom());
    globalVariableModel = new VariableModel(mainProc);
    int mainProgramBlock = 0;//debugger.getDebuggerFrame().getEditor().sourceLanguage.equals("PASCAL") ? 1 : 0;
    program.buildVariables(globalVariableModel, mainProgramBlock, 0);
    stack.addStackListener(this);

    historyModel = new DefaultTableModel() {
        public boolean isCellEditable(int row, int column) {
            return false;
        }
    };
    historyModel.setColumnIdentifiers(new Object[]{
	    Config.MRI_PROC, Config.MRI_LINE, Config.MRI_SOURCE, Config.MRI_PCODE, Config.MRI_FILE});
    
    status = STATUS_RUN;

    readNewLine = false;
}

public void openHistoryFile(String fileName) {
    if (debugger.getDebuggerFrame().isHistoryFile()) {
	    if (out != null) {
		    System.out.println("Closing history file");	    
		    try { out.close(); } catch (IOException e1) { }
	    }
	    if (fileName != null)
   	    try {
		    System.out.println("Opening history file");
		    out = new FileWriter(fileName);
		    out.write(" " + Config.MRI_PROC + "  " + Config.MRI_LINE  + "       " 
		              + Config.MRI_SOURCE  + "                 " + 
			      (getDebugger().getDebuggerFrame().isHistorySource() ? "": (Config.MRI_PCODE + "  ")) + 
			      Config.MRI_FILE+"\n");
	    }
	    catch (IOException e) {
		  System.out.println("Cannot open history file");
	    }
    }
}

private String pad(String original, int length, boolean right) {
        if ( original.length() >= length ) return original.substring(0,length-1);
        StringBuffer paddedStr = new StringBuffer( length );
	if (!right)
            for (int i = original.length(); i < length; i++) paddedStr.append(' ');
        paddedStr.append(original);
	if (right)
            for (int i = original.length(); i < length; i++) paddedStr.append(' ');
        return paddedStr.toString();
}

void updateHistory(BaciProcess currentProcess, PcodeInstruction currentInstruction) {
    Object[] histData = new Object[5];
    histData[0]=new Integer(currentProcess.getIndex());
    int fileIndex = currentInstruction.getFileIndex();
    int rowIndex = currentInstruction.getFileLineIndex();
    String sourceFile = program.getFile(fileIndex).getName();
    histData[1]=new Integer(rowIndex);
    histData[2]=((String) currentProcess.getProgram().getFileSource(fileIndex).get(rowIndex)).trim();
    histData[3]=currentInstruction.toString();
    histData[4]=sourceFile.substring(sourceFile.lastIndexOf(java.io.File.separatorChar)+1);
    if (debugger.getDebuggerFrame().isHistoryFile()) {
           try {
		   out.write(
		   pad(histData[0].toString(), 4, false) + 
		   pad(histData[1].toString(), 6, false) + "    " +
		   pad(histData[2].toString(), 20, true) + "    " +
		   (getDebugger().getDebuggerFrame().isHistorySource() ? "" :
		      pad(histData[3].toString(), 30, true)) +
		   histData[4] + "\n" );
            }
	    catch (IOException e) {
		  System.out.println("Cannot write to history.txt");
	    }
    }
    if(historyModel.getRowCount()==HistoryWindow.MAX_HISTORY_SIZE) {
        historyModel.removeRow(0);
    }
    historyModel.addRow(histData);
}

/**
 * Runs one pcode instruction
 * @throws BaciException if an error occurs in the pcode instruction
 */
public void interperet() throws BaciException {
    if(numInstBeforeSwap==0 || currentProcessIndex==0) {
        int oldProcessIndex = currentProcessIndex;
        //swap procs needed
        if(getProcess(currentProcessIndex).getAtomicCount()==0) {
            currentProcessIndex = chooseProc(0);
            if(currentProcessIndex==-1) {
                setRunning(false);
                throw new BaciException("A deadlock occurred.");
            }
        }
        //randomly select number of instructions to run before next swap
        numInstBeforeSwap = randomGenerator.nextInt(MAX_BEFORE_SWITCH)+1;
        if(oldProcessIndex!=currentProcessIndex) {
            //notify listeners that current process has changed
            fireCurrentProcessChange(new CurrentProcessChangeEvent(this,oldProcessIndex,currentProcessIndex));
            if(stopOnSwap) {
                setRunning(false);
                return;
            }
        }
    }
    if(status!=STATUS_RUN) {
        return;
    }
    BaciProcess currentProcess = processes.get(currentProcessIndex);
    int currentInstructionIndex = currentProcess.getCurrentPcode();
    if(running && !startingRun && breakpointManager.isBreakpoint(currentInstructionIndex,currentProcess)) {
        //at a breakpoint, pause
        setRunning(false);
        return;
    }
    
    // Implement source stepping
    PcodeInstruction currentInstruction = program.getPcode(currentInstructionIndex);
    if (currentInstructionIndex+1 < program.getPcodeSize()) {
        PcodeInstruction nextInstruction = program.getPcode(currentInstructionIndex+1);
        if (sourceStepping && (currentInstruction.getFileLineIndex() != nextInstruction.getFileLineIndex())) {
	    numInstBeforeSwap = 1000; // Prevent automatic swapping
	    setRunning(false);
	    setSourceStepping(false);
        }
    }
    
// If read, wait till after instruction is executed so current instruction indicator not updated till after
//    if(!(currentInstruction instanceof InstructionRead)) {
        currentProcess.setCurrentPcode(currentInstructionIndex+1);
//    }

    numInstBeforeSwap--;

    //add data to history table
    if (debugger.getDebuggerFrame().isHistorySource()) {
	    if (currentInstructionIndex+1 < program.getPcodeSize()) {
		    PcodeInstruction nextInstruction = program.getPcode(currentInstructionIndex+1);
		    if (currentInstruction.getFileLineIndex() != nextInstruction.getFileLineIndex())  
			    updateHistory(currentProcess, currentInstruction);
	    }
    }
    else
       updateHistory(currentProcess, currentInstruction);

    //execute the pcode instruction
    currentInstruction.doInstruction(this);
    //If read, current instruction now
//    if(currentInstruction instanceof InstructionRead) {
//        currentProcess.setCurrentPcode(currentInstructionIndex+1);
//    }
    startingRun = false;
}
/**
 * Determines if this Interpreter is in a cobeing/coend block
 * @return boolean
 */
public boolean isInCobeginBlock() {
    return inCobeginBlock;
}
/**
 * Determines if a new line character was read
 * @return boolean
 */
public boolean isReadNewLine() {
    return readNewLine;
}
/**
 * Determines if this Interpreter is running
 * @return boolean
 */
public boolean isRunning() {
    return running;
}
/**
 * Remove a listener from the list that's notified when the current process changes
 * @param l listener
 */
public void removeCurrentProcessListener(CurrentProcessListener l) {
    currentProcessListeners.remove(l);
}
/**
 * Executes on pcode instruction. Called by run thread
 */
public void run() {
    try {
        //execute one instruction
        interperet();
    } catch (Throwable t) {
        //an error occurred
        //stop running
        setRunning(false);
        //set status to error
        setStatus(STATUS_ERROR);
        //display the error message
        debugger.getDebuggerFrame().showError(t);
    }
}
/**
 * Sets if this Interpreter is in a cobegin/coend block
 * @param newInCobeginBlock boolean
 */
public void setInCobeginBlock(boolean newInCobeginBlock) {
    inCobeginBlock = newInCobeginBlock;
}
/**
 * Sets the number of instructions till the next swap
 * @param num of instructions
 */
public void setNumInstBeforeSwap(int num) {
    numInstBeforeSwap = num;
}
/**
 * Sets if a new line character was read
 * @param newReadNewLine
 */
public void setReadNewLine(boolean newReadNewLine) {
    readNewLine = newReadNewLine;
}
/**
 * Sets if the interpreter is running
 * @param newRunning boolean
 */
public void setRunning(boolean newRunning) {
    running = newRunning;
    startingRun = true;
    synchronized (this) {
        notifyAll();
    }
}
/**
 * Sets pcode address for short call
 * @param newShortCallAddress
 */
public void setShortCallAddress(int newShortCallAddress) {
    shortCallAddress = newShortCallAddress;
}
/**
 * Sets the status of this Interpreter
 * @param newStatus
 */
public void setStatus(int newStatus) {
    status = newStatus;
}
/**
 * Sets if Interpreter should stop on process swap
 * @param newStopOnSwap
 */
public void setStopOnSwap(boolean newStopOnSwap) {
    stopOnSwap = newStopOnSwap;
}
/**
 * Called stack has changed.
 * @param event 
 */
public void stackChanged(StackChangeEvent event) {
    int index = event.getIndex();
    VariableModel.VariableNode node = globalVariableModel.getNodeForStackIndex(index);
    if(node!=null) {
        node.nodeChanged();
    }
}
/**
 * Stops the specified process
 * @param index of process
 */
public void stopProcess(int index) {
    BaciProcess process = processes.get(index);
    process.setActive(false);
    process.setSuspend(null);
    process.setFinished(true);

    numInstBeforeSwap=0;

    boolean activeFound = false;
    for(int i=1; i<processes.getSize(); i++) {
        if(!processes.get(i).isFinished()) {
            activeFound = true;
            break;
        }
    }
    //if all concurrent process are finished, wake up main process
    if(!activeFound) {
        process = processes.get(0);
        process.setActive(true);
        process.setSuspend(null);
    }
}
/**
 * Waits for Interpreter to be running
 */
synchronized public void waitRun() {
    while(!isRunning()) {
        try {
            wait();
        } catch(InterruptedException ie) {
        }
    }
}
}
