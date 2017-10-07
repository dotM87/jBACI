package baci.program;

import baci.interpreter.*;
/**
 * InstructionWrite
 * @author: David Strite
 */
public class InstructionWrite extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWrite(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	String data = null;
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();
	if (stack.get(top) == null) data = "Variable not initialized\n";
	else
	switch(getY()) {
		case Identifier.DATA_TYPE_INTS:
		case Identifier.DATA_TYPE_SEMS:
		case Identifier.DATA_TYPE_BSEMS: {
			data = stack.get(top).toString();
			break;
		}
		case Identifier.DATA_TYPE_BOOLS: {
			if(stack.getInt(top)==0) {
				data = "FALSE";
			} else {
				data = "TRUE";
			}
			break;
		}
		case Identifier.DATA_TYPE_CHARS: {
			data = String.valueOf((char)stack.getInt(top));
			break;
		}
	}
	currProc.setTop(top-1);
	interpreter.getConsole().print(data);
	currProc.getConsole().print(data);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	StringBuffer desc = new StringBuffer();
	desc.append("WRITE ");
	switch(getY()) {
		case Identifier.DATA_TYPE_INTS: {
			desc.append("(int)");
			break;
		}
		case Identifier.DATA_TYPE_SEMS: {
			desc.append("(semaphore)");
			break;
		}
		case Identifier.DATA_TYPE_BSEMS: {
			desc.append("(binarysem)");
			break;
		}
		case Identifier.DATA_TYPE_CHARS: {
			desc.append("(char)");
			break;
		}
		default: {
			desc.append("(??)");
			break;
		}
	}
	desc.append(" s[t] to stdout, pop(1)");
	return desc.toString();
}
}
