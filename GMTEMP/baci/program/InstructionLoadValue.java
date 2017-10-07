package baci.program;

import baci.interpreter.*;
/**
 * InstructionLoadValue
 * @author: David Strite
 */
public class InstructionLoadValue extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionLoadValue(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop()+1;
	currProc.setTop(top);
	Stack stack = currProc.getStack();
	Address address = currProc.getDisplay().getAddress(getX());
	Stack displayStack = address.getProcess().getStack();
	int index = address.getIndex()+getY();
	Object value = displayStack.get(index);
	stack.set(top,value);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "LOAD_VALUE, push "+getProgram().getIdentifier(getBlockIndex(),getX(),getY()).getName();
}
}
