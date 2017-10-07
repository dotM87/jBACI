package baci.program;

import baci.gui.*;
import baci.interpreter.*;
/**
 * InstructionCobegin
 * @author: David Strite
 */
public class InstructionCobegin extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCobegin(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	interpreter.setInCobeginBlock(true);
	interpreter.getDebugger().getDebuggerFrame().getWindowManager().closeProcessWindows(false);
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "COBEGIN";
}
}
