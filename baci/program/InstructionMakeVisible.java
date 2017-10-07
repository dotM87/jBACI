package baci.program;
import baci.interpreter.*;
import baci.graphics.*;

/**
 * InstructionMakeVisible MakeVisibles graphics object
 *
 * @author Moti Ben-Ari
 * @version (20 December 2002)
 * See copyright.txt.
 */
public class InstructionMakeVisible extends PcodeInstruction {

/**
 * Constructor
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionMakeVisible(Program program, int x, int y) {
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
    int handle = stack.getInt(top-1);
    boolean visible = stack.getInt(top) != 0;
    currProc.setTop(top-2);
    GraphicsObject g = interpreter.getGraphicsList().getGraphicsObject(handle);
    if (g != null)
      if (visible) g.makeVisible(); else g.makeInvisible();
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
    return "MakeVisible graphics object, pop(2)";
}
}