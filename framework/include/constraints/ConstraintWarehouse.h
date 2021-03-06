//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CONSTRAINTWAREHOUSE_H
#define CONSTRAINTWAREHOUSE_H

// MOOSE includes
#include "MooseObjectWarehouse.h"
#include "MooseTypes.h"

class Constraint;
class NodalConstraint;
class NodeFaceConstraint;
class MortarConstraint;
class ElemElemConstraint;
class NodeElemConstraint;

/**
 * Warehouse for storing constraints
 */
class ConstraintWarehouse : public MooseObjectWarehouse<Constraint>
{
public:
  ConstraintWarehouse();

  /**
   * Add Constraint object to the warehouse.
   * @param object A std::shared_ptr of the object
   * @param tid Not used.
   */
  void addObject(std::shared_ptr<Constraint> object, THREAD_ID tid = 0, bool recurse = true);

  ///@{
  /**
   * Access methods for active objects.
   */
  const std::vector<std::shared_ptr<NodalConstraint>> & getActiveNodalConstraints() const;
  const std::vector<std::shared_ptr<MortarConstraint>> &
  getActiveMortarConstraints(const std::string & interface) const;
  const std::vector<std::shared_ptr<ElemElemConstraint>> &
  getActiveElemElemConstraints(InterfaceID interface_id, bool displaced) const;
  const std::vector<std::shared_ptr<NodeFaceConstraint>> &
  getActiveNodeFaceConstraints(BoundaryID boundary_id, bool displaced) const;
  const std::vector<std::shared_ptr<NodeElemConstraint>> &
  getActiveNodeElemConstraints(SubdomainID slave_id, SubdomainID master_id, bool displaced) const;
  ///@}

  ///@{
  /**
   * Deterimine if active objects exist.
   */
  bool hasActiveNodalConstraints() const;
  bool hasActiveMortarConstraints(const std::string & interface) const;
  bool hasActiveElemElemConstraints(const InterfaceID interface_id, bool displaced) const;
  bool hasActiveNodeFaceConstraints(BoundaryID boundary_id, bool displaced) const;
  bool
  hasActiveNodeElemConstraints(SubdomainID slave_id, SubdomainID master_id, bool displaced) const;
  ///@}

  /**
   * Update supplied subdomain and variable coverate containters.
   */
  void subdomainsCovered(std::set<SubdomainID> & subdomains_covered,
                         std::set<std::string> & unique_variables,
                         THREAD_ID tid = 0) const;

  /**
   * Update the various active lists.
   */
  void updateActive(THREAD_ID tid = 0);

  virtual void residualEnd(THREAD_ID tid = 0) const;

protected:
  /// NodalConstraint objects
  MooseObjectWarehouse<NodalConstraint> _nodal_constraints;

  /// NodeFaceConstraint objects (non-displaced)
  std::map<BoundaryID, MooseObjectWarehouse<NodeFaceConstraint>> _node_face_constraints;

  /// NodeFaceConstraint objects (displaced)
  std::map<BoundaryID, MooseObjectWarehouse<NodeFaceConstraint>> _displaced_node_face_constraints;

  /// MortarConstraints
  std::map<std::string, MooseObjectWarehouse<MortarConstraint>> _mortar_constraints;

  /// ElemElemConstraints (non-displaced)
  std::map<unsigned int, MooseObjectWarehouse<ElemElemConstraint>> _element_constraints;

  /// ElemElemConstraints (displaced)
  std::map<unsigned int, MooseObjectWarehouse<ElemElemConstraint>> _displaced_element_constraints;

  /// NodeElemConstraint objects
  std::map<std::pair<SubdomainID, SubdomainID>, MooseObjectWarehouse<NodeElemConstraint>>
      _node_elem_constraints;

  /// NodeElemConstraint objects
  std::map<std::pair<SubdomainID, SubdomainID>, MooseObjectWarehouse<NodeElemConstraint>>
      _displaced_node_elem_constraints;
};

#endif // CONSTRAINTWAREHOUSE_H
