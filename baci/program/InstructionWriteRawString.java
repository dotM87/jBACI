package baci.program;

import baci.interpreter.*;
/**
 * InstructionWriteRawString
 * @author: David Strite
 */
public class InstructionWriteRawString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWriteRawString(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	String string = getProgram().getString(getY());
	interpreter.getConsole().print(string);
	interpreter.getCurrentProcess().getConsole().print(string);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "WRITE_RAWSTRING, stab["+getY()+"] to stdout";
}
}
