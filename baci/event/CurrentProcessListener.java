package baci.event;

import java.util.*;
/**
 * Interface for current process listeners
 * @author: David Strite
 */
public interface CurrentProcessListener extends EventListener {
/**
 * Called when the current process changes
 * @param event
 */
void currentProcessChanged(CurrentProcessChangeEvent event);
}
