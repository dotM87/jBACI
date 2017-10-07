package baci.program;

import baci.interpreter.*;
/**
 * InstructionLoadIndirect
 * @author: David Strite
 */
public class InstructionLoadIndirect extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionLoadIndirect(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop()+1;
	currProc.setTop(top);
	Address address = currProc.getDisplay().getAddress(getX());
	address = new Address(address.getProcess(),address.getIndex()+getY());
	address = address.getProcess().getStack().getAddress(address.getIndex());
	Object value = address.getProcess().getStack().get(address.getIndex());
	currProc.getStack().set(top,value);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "LOAD_INDIRECT, push s["+getProgram().getIdentifier(getBlockIndex(),getX(),getY()).getName()+"]";
}
}
