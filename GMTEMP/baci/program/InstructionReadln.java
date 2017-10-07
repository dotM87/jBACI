package baci.program;

import baci.interpreter.*;
/**
 * InstructionReadln
 * @author: David Strite
 */
public class InstructionReadln extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionReadln(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	//Do nothing since not using standard input. Normal interpretter halts if stdin is at eof.
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "READLN";
}
}
