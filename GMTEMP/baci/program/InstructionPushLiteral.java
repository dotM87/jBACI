package baci.program;

import baci.interpreter.*;
/**
 * InstructionPushLiteral
 * @author: David Strite
 */
public class InstructionPushLiteral extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionPushLiteral(Program program, int x, int y) {
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
	currProc.getStack().set(top,getY());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "PUSH_LIT "+getY();
}
}
