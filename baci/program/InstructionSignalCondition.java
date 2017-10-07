package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionSignalCondition
 * @author: David Strite
 */
public class InstructionSignalCondition extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionSignalCondition(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if monitor has not been initialized
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	Address monitorAddress = currProc.getMonitor();
	Object value = monitorAddress.getProcess().getStack().get(monitorAddress.getIndex());
	if(value==null) {
		throw new BaciException("Monitor is not initialized");
	} else {
		int top = currProc.getTop();
		Address suspendAddress = stack.getAddress(top);
		top--;
		currProc.setTop(top);

		BaciProcess selectedProc = null;
		BaciProcess lookProc = null;

		ProcessList processes = interpreter.getProcesses();
		int numProcs = processes.getSize();
		int current = interpreter.getRandomInt(numProcs);
		int count = 0;
		int minPriority = 0x7FFF;
		for(int i=0; i<numProcs; i++) {
			lookProc = processes.get(current);
			if(suspendAddress.equals(lookProc.getSuspend()) && monitorAddress.equals(lookProc.getMonitor())) {
				if(lookProc.getPriority()<minPriority) {
					minPriority = lookProc.getPriority();
					selectedProc = lookProc;
				}
			}
			current = (current+1)%numProcs;
		}
		if(selectedProc!=null) {
			currProc.setActive(false);
			currProc.setSuspend(monitorAddress);
			currProc.setPriority(-1);  // Resume signaling process first
			interpreter.setNumInstBeforeSwap(0);
			selectedProc.setActive(true);
			selectedProc.setSuspend(null);
		}
	}
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SIGNALC condition &s[t], pop(1)";
}
}
