package baci.graphics;
import java.util.*;

/**
 * Store graphics objects and execute graphics commands
 *
 * @author Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class GraphicsList {

    private ArrayList graphics;

public GraphicsList() {
    graphics = new ArrayList();
}

// Create and add a graphics object to the list.
// Parameter f gives the type of object to create.
public void create(int h, int f, int c, int xp, int yp, int xs, int ys) {
    switch (f) {
	    case 1: graphics.add(new Circle(h, c, xp, yp, xs, ys));    break;
            case 2: graphics.add(new Line(h, c, xp, yp, xs, ys));      break;
            case 3: graphics.add(new Rectangle(h, c, xp, yp, xs, ys)); 	break;
            case 4: graphics.add(new Triangle(h, c, xp, yp, xs, ys)); 	break;
    }
}

public void clear() {
	for (int i = 0; i < graphics.size(); i++)
		((GraphicsObject)graphics.get(i)).erase();
	Canvas.destroy();
	graphics.clear();
}

// Find a graphics object.
// indexOf uses overridden equals method.
public GraphicsObject getGraphicsObject(int handle) {
	int index = graphics.indexOf(new GraphicsObject(handle));
    if (index == -1) {
        System.out.println("Graphics object " + handle + " not found ");
        return null;
    }
    else
        return (GraphicsObject) graphics.get(index);
}

}
