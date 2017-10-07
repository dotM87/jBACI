package baci.program;

import baci.interpreter.*;
/**
 * InstructionEmpty
 * @author: David Strite
 */
public class InstructionEmpty extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionEmpty(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();

	Address address = stack.getAddress(currProc.getTop());
	boolean empty = true;
	ProcessList processes = interpreter.getProcesses();

	//search through processes for one suspended on this address
	for(int i=1; i<processes.getSize(); i++) {
		if(address.equals(processes.get(i).getSuspend())) {
			empty = false;
			break;
		}
	}
	stack.set(currProc.getTop(),empty?1:0);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "EMPTY, condition &s[t]";
}
}
