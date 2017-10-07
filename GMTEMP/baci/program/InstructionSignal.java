package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionSignal
 * @author: David Strite
 */
public class InstructionSignal extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionSignal(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the value of binary semaphore is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException{
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();

	int top = currProc.getTop();
	Address address = stack.getAddress(top);
	top--;
	currProc.setTop(top);

	ProcessList processes = interpreter.getProcesses();
	int numProcs = processes.getSize();
	int current = interpreter.getRandomInt(numProcs);
	int count = 0;
	while(!address.equals(processes.get(current).getSuspend()) && count<numProcs) {
		current = (current+1)%numProcs;
		count++;
	}
	if(count==numProcs) {
		//no one waiting
		int oldValue = address.getProcess().getStack().getInt(address.getIndex());
		address.getProcess().getStack().set(address.getIndex(),oldValue+1);
		if(getY()!=0 && oldValue+1>1) {
			//bsem check
			throw new BaciException("Invalid binary semaphore value");
		}
	} else {
		BaciProcess wakeProc = processes.get(current);
		wakeProc.setSuspend(null);
		wakeProc.setActive(true);
	}
		
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SIGNAL(s[s[t]]), pop(1)";
}
}
