package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionWaitCondition
 * @author: David Strite
 */
public class InstructionWaitCondition extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWaitCondition(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the monitor has not been initialized
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	Address monitorAddress = currProc.getMonitor();
	Object value = monitorAddress.getProcess().getStack().get(monitorAddress.getIndex());
	if(value==null) {
		throw new BaciException("Monitor has not been initialized");
	} else {
		int top = currProc.getTop();
		currProc.setSuspend(stack.getAddress(top-1));
		currProc.setPriority(stack.getInt(top));
		top-=2;
		currProc.setTop(top);
		currProc.setActive(false);
		interpreter.setNumInstBeforeSwap(0);

		ProcessList processes = interpreter.getProcesses();
		int numProcs = processes.getSize();
		int current = interpreter.getRandomInt(numProcs);
		int count = 0;
		while((!monitorAddress.equals(processes.get(current).getMonitor()) || !monitorAddress.equals(processes.get(current).getSuspend())) && count<numProcs) {
			current = (current+1)%numProcs;
			count++;
		}
		if(count==numProcs) {
			int newValue = monitorAddress.getProcess().getStack().getInt(monitorAddress.getIndex())+1;
			monitorAddress.getProcess().getStack().set(monitorAddress.getIndex(),newValue);
		} else {
			BaciProcess wakeProc = processes.get(current);
			wakeProc.setSuspend(null);
			wakeProc.setActive(true);
		}
		
	}
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "WAITC, priority s[t], condition &s[t-1], pop(2)";
}
}
