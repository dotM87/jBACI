package baci.event;

import java.util.*;
/**
 * Interface for current pcode listeners
 * @author: David Strite
 */
public interface CurrentPcodeListener extends EventListener {
/**
 * Called when the current pcode instruction changes
 * @param event
 */
void currentPcodeChanged(CurrentPcodeChangeEvent event);
}
