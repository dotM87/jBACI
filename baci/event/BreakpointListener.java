package baci.event;

import java.util.*;
/**
 * Interface for break point listeners
 * @author: David Strite
 */
public interface BreakpointListener extends EventListener {
/**
 * Called when a break point is changed
 * @param event
 */
void breakpointChanged(BreakpointChangeEvent event);
}
