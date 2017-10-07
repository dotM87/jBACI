package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionEndFor
 * @author: David Strite
 */
public class InstructionEndFor extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionEndFor(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if invalid pcode address is encountered
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();

	Address address = stack.getAddress(top-2);
	int newValue = address.getProcess().getStack().getInt(address.getIndex())+1;
	int max = stack.getInt(top);
	if(newValue<=max) {
		address.getProcess().getStack().set(address.getIndex(),newValue);
		getProgram().validatePcodeAddress(getY());
		currProc.setCurrentPcode(getY());
	} else {
		top-=3;
		currProc.setTop(top);
	}
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "ENDFOR, s[s[t-2]]++, if(s[t-1]<=s[t]) then s[s[t-2]]=s[t-1], JUMP "+getY()+" else pop(3)";
}
}
