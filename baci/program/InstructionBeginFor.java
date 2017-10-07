package baci.program;

import baci.exception.*;
import baci.interpreter.*;

/**
 * InstructionBeginFor
 * @author: David Strite
 */
 
public class InstructionBeginFor extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionBeginFor(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if y is not a valid pcode address
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();

	int current = stack.getInt(top-1);
	int max = stack.getInt(top);
	if(current<=max) {
		//stay in for loop
		Address address = stack.getAddress(top-2);
		address.getProcess().getStack().set(address.getIndex(),current);
	} else {
		//for loop is finished
		top-=3;
		currProc.setTop(top);
		getProgram().validatePcodeAddress(getY());
		currProc.setCurrentPcode(getY());
	}
	
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "BEGINFOR, if(s[t-1]<=s[t]) then s[s[t-2]]=s[t-1] else pop(3), JUMP "+getY();
}
}
