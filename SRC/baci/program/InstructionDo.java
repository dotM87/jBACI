package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionDo is superclass of Do* classes which are arithmetic and boolean instructions
 * @author: David Strite
 */
abstract public class InstructionDo extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionDo(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if error occurred in doOp
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop()-1;
	currProc.setTop(top);
	Stack stack = currProc.getStack();
	stack.set(top,doOp(stack.getInt(top),stack.getInt(top+1)));
}
/**
 * Performs instruction's operation on the two arguments
 * @param arg1 int
 * @param arg2 int
 */
protected abstract int doOp(int arg1, int arg2) throws BaciException;
}
