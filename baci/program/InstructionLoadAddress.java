package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionLoadAddress
 * @author: David Strite
 */
public class InstructionLoadAddress extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionLoadAddress(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop()+1;
	currProc.setTop(top);
	Address address = currProc.getDisplay().getAddress(getX());
	currProc.getStack().set(top,new Address(address.getProcess(),address.getIndex()+getY()));
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "LOAD_ADDR, push "+getProgram().getIdentifier(getBlockIndex(),getX(),getY()).getName();
}
}
