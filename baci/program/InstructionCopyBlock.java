package baci.program;

import baci.interpreter.*;
/**
 * InstructionCopyBlock
 * @author: David Strite
 */
public class InstructionCopyBlock extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCopyBlock(Program program, int x, int y) {
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

	Address toAddress=stack.getAddress(top-1);
	Address fromAddress=stack.getAddress(top);
	int stopIndex = toAddress.getIndex()+getY();

	while(toAddress.getIndex()<stopIndex) {
		toAddress.getProcess().getStack().set(toAddress.getIndex(),fromAddress.getProcess().getStack().get(fromAddress.getIndex()));
		toAddress.setIndex(toAddress.getIndex()+1);
		fromAddress.setIndex(fromAddress.getIndex()+1);
	}
	top-=2;
	currProc.setTop(top);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "COPY_BLOCK, copy "+getY()+" words from &s[t] to &s[t-1], pop(2)";
}
}
