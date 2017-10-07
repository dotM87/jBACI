package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionEolEof
 * @author: David Strite
 */
public class InstructionEolEof extends PcodeInstruction {
	public static final int EOF = 17;
	public static final int EOL = 18;
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionEolEof(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if y is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop()+1;
	currProc.setTop(top);
	
	switch (getY()) {
		case EOF:
			stack.set(top,0);
			break;
		case EOL:
			stack.set(top,interpreter.isReadNewLine()?1:0);
			break;
		default:
			throw new BaciException("Invalid parameter: "+getY()+" to EOL/EOF");
	}
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	switch (getY()) {
		case EOF: {
			return "SF_EOF, push EOF(stdin)";
		}
		case EOL: {
			return "SF_EOLN, push EOLN(stdin)";
		}
	}
	return "unknown y field";
}
}
