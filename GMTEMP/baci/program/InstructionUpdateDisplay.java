package baci.program;

import baci.interpreter.*;
/**
 * InstructionUpdateDisplay
 * @author: David Strite
 */
public class InstructionUpdateDisplay extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionUpdateDisplay(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int index = getY();
	int stopIndex = getX();
	int bottom = currProc.getBottom();

	Stack display = currProc.getDisplay();
	Stack stack = currProc.getStack();
	do {
		display.set(index,new Address(currProc,bottom));
		index--;
		Address address = stack.getAddress(bottom+2);
		bottom=address.getIndex();
		stack = address.getProcess().getStack();
	} while (index!=stopIndex);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "UPDATE_DISPLAY from level "+getX()+" out to level "+getY();
}
}
