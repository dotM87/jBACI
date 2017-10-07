package baci.graphics;

import java.awt.*;
import java.awt.geom.*;

/**
 * @author  Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */

class Line extends GraphicsObject {

    // Create a new line from position x,y to position xe,ye of color c.
    Line(int handle, int c, int x, int y, int xe, int ye)
    {
        super(handle, 2, c, x, y, xe, ye);
    }

    // Move the line to a new position.
    public void moveTo(int newX, int newY)
    {
        erase();
        xSize     = (xSize-xPosition) + newX;
        ySize     = (ySize-yPosition) + newY;
        xPosition = newX;
        yPosition = newY;
        draw();
    }

    // Move the line by an amount.
    public void moveBy(int deltaX, int deltaY)
    {
        erase();
        xPosition = xPosition + deltaX;
        yPosition = yPosition + deltaY;
        xSize     = xSize     + deltaX;
        ySize     = ySize     + deltaY;
        draw();
    }

    /*
     * Draw the line with current specifications on screen.
     */
    protected void draw()
    {
        if(isVisible) {
            Canvas canvas = Canvas.getCanvas();
            canvas.draw(this, color,
                new Line2D.Double(
                    (double) xPosition, (double) yPosition,
                    (double) xSize, (double) ySize));
            canvas.wait(10);
        }
    }

}
