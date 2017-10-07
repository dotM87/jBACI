package baci.graphics;

import java.awt.*;
import java.awt.geom.*;

/**
 * This program has been modified by Moti Ben-Ari (December 2002).
 * The original was taken from the BlueJ examples.
 * There was not copyright on the original.
 * A rectangle that can be manipulated and that draws itself on a canvas.
 *
 * @author  Michael Kolling and David J. Barnes
 * @version 1.0  (15 July 2000)
 */

public class Rectangle extends GraphicsObject {

    // Create a new rectangle of size s, at position x,y of color c.
    Rectangle(int handle, int c, int x, int y, int w, int h)
    {
        super(handle, 3, c, x, y, w, h);
    }

    /*
     * Draw the rectangle with current specifications on screen.
     */
    protected void draw()
    {
        if(isVisible) {
            Canvas canvas = Canvas.getCanvas();
            canvas.draw(this, color,
                new Rectangle2D.Double(
                    (double) xPosition, (double) yPosition,
                    (double) xSize, (double) ySize));
            canvas.wait(10);
        }
    }

}
