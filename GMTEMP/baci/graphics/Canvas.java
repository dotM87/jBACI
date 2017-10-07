package baci.graphics;

import baci.gui.Config;
import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.*;

/**
 * This program has been modified by Moti Ben-Ari (December 2002).
 * The original was taken from the BlueJ examples.
 * There was not copyright on the original.
 *
 * Canvas is a class to allow for simple graphical drawing on a canvas.
 * This is a modification of the general purpose Canvas, specially made for
 * the BlueJ "shapes" example.
 *
 * @author: Bruce Quig
 * @author: Michael Kolling (mik)
 *
 * @version: 1.6 (shapes)
 */
public class Canvas
{
    // Note: The implementation of this class (specifically the handling of
    // shape identity and colors) is slightly more complex than necessary. This
    // is done on purpose to keep the interface and instance fields of the
    // shape objects in this project clean and simple for educational purposes.

    private static Canvas canvasSingleton;

    /**
     * Factory method to get the canvas singleton object.
     */
    public static Canvas getCanvas()
    {
        if(canvasSingleton == null) {
            canvasSingleton = new Canvas(
	    	Config.G_TITLE, 
		Config.getIntegerProperty("G_XPOS"), 
		Config.getIntegerProperty("G_YPOS"), 
		Config.getIntegerProperty("G_WIDTH"), 
		Config.getIntegerProperty("G_HEIGHT"), 
		Config.G_COLOR);
        }
        canvasSingleton.setVisible(true);
        return canvasSingleton;
    }

    public static void destroy() {
	    if (canvasSingleton != null) {
		    canvasSingleton.erase();
		    canvasSingleton.frame.dispose();
	    }
    }
    
    //  ----- instance part -----

    private JFrame frame;
    private CanvasPane canvas;
    private Graphics2D graphic;
    private Color backgroundColour;
    private Image canvasImage;
    private List objects;
    private HashMap shapes;

    /**
     * Create a Canvas.
     * @param title  title to appear in Canvas Frame
     * @param width  the desired width for the canvas
     * @param height  the desired height for the canvas
     * @param bgClour  the desired background colour of the canvas
     */
    private Canvas(String title, int xPos, int yPos, int width, int height, Color bgColour)
    {
        frame = new JFrame();
        canvas = new CanvasPane();
        frame.setContentPane(canvas);
        frame.setTitle(title);
        canvas.setPreferredSize(new Dimension(width, height));
        frame.setLocation(xPos, yPos);
        backgroundColour = bgColour;
        frame.pack();
        objects = new ArrayList();
        shapes = new HashMap();
    }

    /**
     * Set the canvas visibility and brings canvas to the front of screen
     * when made visible. This method can also be used to bring an already
     * visible canvas to the front of other windows.
     * @param visible  boolean value representing the desired visibility of
     * the canvas (true or false)
     */
    public void setVisible(boolean visible)
    {
        if(graphic == null) {
            // first time: instantiate the offscreen image and fill it with
            // the background colour
            Dimension size = canvas.getSize();
            canvasImage = canvas.createImage(size.width, size.height);
            graphic = (Graphics2D)canvasImage.getGraphics();
            graphic.setColor(backgroundColour);
            graphic.fillRect(0, 0, size.width, size.height);
            graphic.setColor(Color.black);
        }
        frame.setVisible(visible);
    }

    /**
     * Draw a given shape onto the canvas.
     * @param  referenceObject  an object to define identity for this shape
     * @param  color            the color of the shape
     * @param  shape            the shape object to be drawn on the canvas
     */
     // Note: this is a slightly backwards way of maintaining the shape
     // objects. It is carefully designed to keep the visible shape interfaces
     // in this project clean and simple for educational purposes.
    public void draw(Object referenceObject, int color, Shape shape)
    {
        objects.remove(referenceObject);   // just in case it was already there
        objects.add(referenceObject);      // add at the end
        shapes.put(referenceObject, new ShapeDescription(shape, color));
        redraw();
    }
 
    /**
     * Erase a given shape's from the screen.
     * @param  referenceObject  the shape object to be erased
     */
    public void erase(Object referenceObject)
    {
        objects.remove(referenceObject);   // just in case it was already there
        shapes.remove(referenceObject);
        redraw();
    }

    /**
     * Set the foreground colour of the Canvas.
     * @param  newColour   the new colour for the foreground of the Canvas
     */
    public void setForegroundColor(int colorInt)
    {
        if(colorInt==1)
            graphic.setColor(Color.red);
        else if(colorInt==2)
            graphic.setColor(Color.black);
        else if(colorInt==3)
            graphic.setColor(Color.blue);
        else if(colorInt==4)
            graphic.setColor(Color.yellow);
        else if(colorInt==5)
            graphic.setColor(Color.green);
        else if(colorInt==6)
            graphic.setColor(Color.magenta);
        else if(colorInt==7)
            graphic.setColor(Color.white);
        else
            graphic.setColor(Color.black);
    }

    /**
     * Wait for a specified number of milliseconds before finishing.
     * This provides an easy way to specify a small delay which can be
     * used when producing animations.
     * @param  milliseconds  the number
     */
    public void wait(int milliseconds)
    {
        try
        {
            Thread.sleep(milliseconds);
        }
        catch (Exception e)
        {
            // ignoring exception at the moment
        }
    }

    /**
     * Redraw ell shapes currently on the Canvas.
     */
    private void redraw()
    {
        erase();
        for(Iterator i=objects.iterator(); i.hasNext(); ) {
            ((ShapeDescription)shapes.get(i.next())).draw(graphic);
        }
        canvas.repaint();
    }

    /**
     * Erase the whole canvas. (Does not repaint.)
     */
    public void erase()
    {
        Color original = graphic.getColor();
        graphic.setColor(backgroundColour);
        Dimension size = canvas.getSize();
        graphic.fill(new java.awt.Rectangle(0, 0, size.width, size.height));
        graphic.setColor(original);
    }


    /************************************************************************
     * Inner class CanvasPane - the actual canvas component contained in the
     * Canvas frame. This is essentially a JPanel with added capability to
     * refresh the image drawn on it.
     */
    private class CanvasPane extends JPanel
    {
        public void paint(Graphics g)
        {
            g.drawImage(canvasImage, 0, 0, null);
        }
    }

    private class ShapeDescription
    {
        private Shape shape;
        private int colorInt;

        public ShapeDescription(Shape shape, int color)
        {
            this.shape = shape;
            colorInt = color;
        }

        public void draw(Graphics2D graphic)
        {
            setForegroundColor(colorInt);
            if (shape instanceof java.awt.geom.Line2D.Double) graphic.draw(shape);
            else graphic.fill(shape);
        }
    }

}
