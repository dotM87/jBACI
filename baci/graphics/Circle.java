package baci.graphics;

import java.awt.*;
import java.awt.geom.*;

/**
 * This program has been modified by Moti Ben-Ari (December 2002).
 * The original was taken from the BlueJ examples.
 * There was not copyright on the original.
 *
 * A circle that can be manipulated and that draws itself on a canvas.
 *
  * @author  Michael Kolling and David J. Barnes
 * @version 1.0  (15 July 2000)
 */

public class Circle extends GraphicsObject {

    /**
      Create a new circle of diameter d, at position x,y of color c.
    */
    Circle(int handle, int c, int x, int y, int d, int dummy)
    {
        super(handle, 1, c, x, y, d, d);
    }

    /*
     * Draw the circle with current specifications on screen.
     */
    protected void draw()
    {
        if(isVisible) {
            Canvas canvas = Canvas.getCanvas();
            canvas.draw(this, color,
                new Ellipse2D.Double(
                    (double) xPosition, (double) yPosition,
                    (double) xSize, (double) ySize));
            canvas.wait(10);
        }
    }

}
