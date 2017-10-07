package baci.event;

import java.util.*;
/**
 * Interface for stack listeners
 * @author: David Strite
 */
public interface StackListener extends EventListener {
/**
 * Called when the stack changes
 * @param event
 */
void stackChanged(StackChangeEvent event);
}
