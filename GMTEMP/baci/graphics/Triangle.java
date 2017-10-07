package baci.graphics;

import java.awt.*;

/**
 * This program has been modified by Moti Ben-Ari (December 2002).
 * The original was taken from the BlueJ examples.
 * There was not copyright on the original.
 *
 * A triangle that can be manipulated and that draws itself on a canvas.
 *
 * @author  Michael Kolling and David J. Barnes
 * @version 1.0  (15 July 2000)
 */

public class Triangle extends GraphicsObject {

    // Create a new triangle of size s, at position x,y of color c.
    Triangle(int handle, int c, int x, int y, int w, int h)
    {
        super(handle, 4, c, x, y, w, h);
    }

    /*
     * Draw the triangle with current specifications on screen.
     */
    protected void draw()
    {
        if(isVisible) {
            Canvas canvas = Canvas.getCanvas();
            int[] xpoints = { xPosition, xPosition + (xSize/2), xPosition - (xSize/2) };
            int[] ypoints = { yPosition, yPosition + ySize, yPosition + ySize };
            canvas.draw(this, color, new Polygon(xpoints, ypoints, 3));
            canvas.wait(10);
        }
    }

}
