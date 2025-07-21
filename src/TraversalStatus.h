/*
 * Created on: Jul 19, 2025
 * By:         Eric Mintz
 *
 * Traversal status that determines behavior. See
 * enumerations for details.
 */

#ifndef TRAVERSAL_STATUS_H_
#define TRAVERSAL_STATUS_H_

namespace VisitingParseTree {

enum class TraversalStatus {
  /*
   * Continue running the traversal. Process children
   * after entry; return after exit.
   */
  CONTINUE,
  /*
   * Bypass processing children on exit; equivalent to
   * CONtiNUE on exit.
   */
  BYPASS_CHILDREN,
  /*
   * Cancel the traversal immediately. The traversal stops
   * WITHOUT invoking additional actions. In particular,
   * if if returned on entry, the bypass both the node's
   * children AND exit processing.
   */
  CANCEL,
};

}  /* Namespace VisitingParseTree */

#endif /* TRAVERSAL_STATUS_H_ */
