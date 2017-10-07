package baci.program;
import baci.interpreter.*;
import baci.graphics.*;

/**
 * InstructionMoveTo MoveTos graphics object
 *
 * @author Moti Ben-Ari
 * @version (20 December 2002)
 * See copyright.txt.
 */
public class InstructionMoveTo extends PcodeInstruction {

/**
 * Constructor
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionMoveTo(Program program, int x, int y) {
    super(program, x, y);
}

/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
    BaciProcess currProc = interpreter.getCurrentProcess();
    Stack stack = currProc.getStack();
    int top = currProc.getTop();
    int handle = stack.getInt(top-2);
    int xPosition = stack.getInt(top-1);
    int yPosition = stack.getInt(top);
    currProc.setTop(top-3);
    GraphicsObject g = interpreter.getGraphicsList().getGraphicsObject(handle);
    if (g != null) g.moveTo(xPosition, yPosition);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
    return "MoveTo graphics object, pop(3)";
}
}
