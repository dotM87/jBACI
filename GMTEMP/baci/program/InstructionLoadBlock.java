package baci.program;

import baci.interpreter.*;
/**
 * InstructionLoadBlock
 * @author: David Strite
 */
public class InstructionLoadBlock extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionLoadBlock(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();

	Address fromAddress = stack.getAddress(top);
	top--;
	int stopIndex = getY()+top;
	while(top<stopIndex) {
		top++;
		stack.set(top,fromAddress.getProcess().getStack().get(fromAddress.getIndex()));
		fromAddress.setIndex(fromAddress.getIndex()+1);
	}
	currProc.setTop(top);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "LOAD_BLOCK, pop(1), push "+getY()+" words at &s[oldt]";
}
}
