package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionPost
 * @author: Moti Ben-Ari
 */
public class InstructionPost extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionPost(Program program, int x, int y) {
    super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the value of binary semaphore is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException{
    BaciProcess currProc = interpreter.getCurrentProcess();
    Stack stack = currProc.getStack();
    Linda linda = interpreter.getLinda();
    int top = currProc.getTop();
    int v1 = stack.getInt(top-2);
    int v2 = stack.getInt(top-1);
    int v3 = stack.getInt(top);
    currProc.setTop(top-3);
    linda.postnote(v1, v2, v3);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
    return "POST, pop(3)";
}
}
