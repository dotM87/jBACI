package baci.program;
import baci.interpreter.*;
import baci.graphics.*;

/**
 * InstructionCreate creates graphics object
 *
 * @author Moti Ben-Ari
 * @version (20 December 2002)
 * See copyright.txt.
 */
public class InstructionCreate extends PcodeInstruction {

/**
 * Constructor
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCreate(Program program, int x, int y) {
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
    int handle = stack.getInt(top-6);
    int figure = stack.getInt(top-5);
    int color = stack.getInt(top-4);
    int xPosition = stack.getInt(top-3);
    int yPosition = stack.getInt(top-2);
    int xSize = stack.getInt(top-1);
    int ySize = stack.getInt(top);
    currProc.setTop(top-7);
    interpreter.getGraphicsList().create(handle, figure, color, xPosition, yPosition, xSize, ySize);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
    return "CREATE graphics object, pop(7)";
}
}
