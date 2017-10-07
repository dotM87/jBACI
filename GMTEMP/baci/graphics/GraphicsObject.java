package baci.graphics;

/**
 * Abstract class for a graphics object
 *
 * @author Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */

public class GraphicsObject {
    int handle;
    int figure;
    int color;
    int xPosition, yPosition;
    int xSize, ySize;
    protected boolean isVisible;

    GraphicsObject(int h, int f, int c, int xp, int yp, int xs, int ys) {
        handle = h; figure = f; color = c;
        xPosition = xp; yPosition = yp; xSize = xs; ySize = ys;
        makeVisible();
    }

    // Construct a trivial object just for comparing handles.
    GraphicsObject(int h) {
        handle = h;
    }

    // Objects are equal iff handles are equal.
    public boolean equals(Object g) {
       return this.handle == ((GraphicsObject) g).handle;
    }

    /**
     * Make this graphics object visible. If it was already visible, do nothing.
     */
    public void makeVisible()
    {
        isVisible = true;
        draw();
    }

    /**
     * Make this graphics object invisible. If it was already invisible, do nothing.
     */
    public void makeInvisible()
    {
        erase();
        isVisible = false;
    }

    /**
     * Change the color. Valid colors are "red", "yellow", "blue", "green",
     * "magenta" and "black".
     */
    public void changeColor(int newColor)
    {
        color = newColor;
        draw();
    }

    // Move the graphics object to a new position.
    public void moveTo(int newX, int newY)
    {
        erase();
        xPosition = newX;
        yPosition = newY;
        draw();
    }

    // Move the graphics object by an amount.
    public void moveBy(int deltaX, int deltaY)
    {
        erase();
        xPosition = xPosition + deltaX;
        yPosition = yPosition + deltaY;
        draw();
    }

    /**
     * Change the size to the new width/height (in pixels), which must be >= 0.
     */
    public void changeSize(int newWidth, int newHeight)
    {
        erase();
        xSize = newWidth;
        ySize = newHeight;
        draw();
    }

    /*
     * Draw the graphics object with current specifications on screen.
     */
    protected void draw()
    {
    }

    /*
     * Erase the graphics object on screen.
     */
    protected void erase()
    {
        if(isVisible) {
            Canvas canvas = Canvas.getCanvas();
            canvas.erase(this);
        }
    }

}

