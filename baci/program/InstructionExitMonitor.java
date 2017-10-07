package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionExitMonitor
 * @author: David Strite
 */
public class InstructionExitMonitor extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionExitMonitor(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if a cross monitor call is encountered or if the monitor has not been initialized
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	Program program = getProgram();
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	BaciProcess selectedProc = null;
	BaciProcess lookProc = null;
	int bottom = currProc.getBottom();
	int procIdentIndex = stack.getInt(bottom+4);
	Address temp = stack.getAddress(bottom+3);
	int callerIdentIndex = temp.getProcess().getStack().getInt(temp.getIndex()+4);
	
	if (program.getIdentifier(callerIdentIndex).getMon() != 0) {
		/* Problem with calling a monitor procedure from the init code */
		if (program.getIdentifier(callerIdentIndex).getObj() == Identifier.OBJ_TYPE_MONITOR) {
			if (program.getIdentifier(procIdentIndex).getMon() != callerIdentIndex) {
				throw new BaciException("Cannot call a monitor from another monitor");
			}
		} else {
			if (program.getIdentifier(procIdentIndex).getMon() != program.getIdentifier(callerIdentIndex).getMon()) {
				throw new BaciException("Cannot call a monitor from another monitor");
			}
		}
	} else {
		Address address = currProc.getMonitor();
		Object value = address.getProcess().getStack().get(address.getIndex());
		if (value == null) {
			throw new BaciException("Monitor has not been initialized");
		} else {
			currProc.setMonitor(null, null);
			ProcessList processes = interpreter.getProcesses();
			// Signaling processes are given a priority of -1 so they will be awakened first at monitor exit
			int numProcs = processes.getSize();
			int current = interpreter.getRandomInt(numProcs);
			int count = 0;
			int minPriority = 0x7FFF;
			for(int i=0; i<numProcs; i++) {
				lookProc = processes.get(current);
				if(address.equals(lookProc.getSuspend()) 
					&& address.equals(lookProc.getMonitor())) {
					if(lookProc.getPriority()<minPriority) {
						minPriority = lookProc.getPriority();
						selectedProc = lookProc;
					}
				}
				current = (current+1)%numProcs;
			}
			if (selectedProc == null) {
				//no one waiting
				int newValue = address.getProcess().getStack().getInt(address.getIndex())+1;
				address.getProcess().getStack().set(address.getIndex(),newValue);
			} else {
				selectedProc.setSuspend(null);
				selectedProc.setActive(true);
				if (selectedProc.getPriority()==-1) selectedProc.setPriority(0);
			}
		}
	}
	
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "EXITMON";
}
}
