package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionIndex
 * @author: David Strite
 */
public class InstructionIndex extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionIndex(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the index is out of bounds
 */
public void doInstruction(Interpreter interpreter) throws BaciException{
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();
	ArrayDimension array = getProgram().getArray(getY());
	int low = array.getLowIndex();
	int index = stack.getInt(top);
	if(index<low || index>array.getHighIndex()) {
		throw new BaciException("Index out of bounds");
	}
	top--;
	currProc.setTop(top);
	Address address = stack.getAddress(top);
	stack.set(top,new Address(address.getProcess(),address.getIndex()+(index-low)*array.getElementSize()));
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "INDEX atab["+getY()+"], pop(1)";
}
}
