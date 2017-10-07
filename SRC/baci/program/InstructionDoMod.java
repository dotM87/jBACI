package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionDoMod
 * @author: David Strite
 */
public class InstructionDoMod extends InstructionDo {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionDoMod(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs instruction's operation on the two arguments
 * @param arg1 int
 * @param arg2 int
 * @throws BaciException if division by 0
 */
protected int doOp(int arg1, int arg2) throws BaciException {
	if(arg2==0) {
		throw new BaciException("Division by 0");
	}
	return arg1%arg2;
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "DO_MOD, pop(1), s[t]=(s[oldt-1]%s[oldt])";
}
}
