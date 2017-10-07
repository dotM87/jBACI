package baci.event;

import java.util.*;
/**
 * Interface for process listeners
 * @author: David Strite
 */
public interface ProcessListener extends EventListener {
/**
 * Called when a process property changes
 * @param event
 */
void processChanged(ProcessChangeEvent event);
}
