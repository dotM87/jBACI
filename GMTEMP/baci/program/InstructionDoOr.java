package baci.program;

import baci.interpreter.*;
/**
 * InstructionDoOr
 * @author: David Strite
 */
public class InstructionDoOr extends InstructionDo {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionDoOr(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Insert the method's description here.
 * Creation date: (9/11/2001 8:54:47 PM)
 * @return int
 * @param arg1 int
 * @param arg2 int
 */
protected int doOp(int arg1, int arg2) {
	boolean temp1 = arg1==0?false:true;
	boolean temp2 = arg2==0?false:true;
	return temp1 || temp2?1:0;
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "DO_OR, pop(1), s[t]=(s[oldt-1]|s[oldt])";
}
}
