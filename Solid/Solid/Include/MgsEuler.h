#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsEuler.h
//
//		ÉIÉCÉâÅ[
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		MgsEuler.h
 *
 *\brief        Include all head files defining node types in euler.
 *
 *              They are solid, face, loop, edge, and vertex nodes.
 *
 *              Some significant change from the original Geometric WorkBench as 
 *              presented by Martti Mantyla as in "An Introduction to Solid Modeling".
 *
 *              0. First, this is a C++ implementation. And as such, it comes with	
 *                 many advantages; among them, most obviously, are the simpler and	
 *                 cleaner codes for node creation and deletion, including the proper	
 *		   link update.								
 *              									
 *              1. From a *pure perspective* of topological plane model of a closed	
 *                 2-manifold, there are actually no such things as edges, instead,	
 *                 there are only half edges, with the condition each is identified to	
 *		   exactly another one. Therefore, there is no edge nodes in my		
 *                 design. The identification is archived by storing the opposite half	
 *                 edge in the half edge node (member o).				
 *											
 *              2. All half edges, including circular edge (end points coincident), have
 *                 their opposite halves, except singular half edge, i.e., of zero length,
 *                 which has nil as its opposite half edge link.
 * 
 *              3. Because there are only half edges, we simple use MgsEdge to denote the 
 *                 actually half edge type.
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		14 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "MgsSolid.h"
#include "MgsFace.h"
#include "MgsLoop.h"
#include "MgsEdge.h"
#include "MgsVertex.h"


