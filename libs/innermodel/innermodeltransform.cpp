/*
 * Copyright 2016 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "innermodeltransform.h"

InnerModelTransform::InnerModelTransform(QString id_, QString engine_, float tx_, float ty_, float tz_, float rx_, float ry_, float rz_, float mass_, NodePtr parent_) : InnerModelNode(id_, parent_)
{
#if FCL_SUPPORT==1
	collisionObject = NULL;
#endif
	engine = engine_;
	set(rx_, ry_, rz_, tx_, ty_, tz_);
	mass = mass_;
	backtX = tx_;
	backtY = ty_;
	backtZ = tz_;
	backrX = rx_;
	backrY = ry_;
	backrZ = rz_;
	gui_translation = gui_rotation = true;
}

// InnerModelTransform::~InnerModelTransform()
// {
// }

void InnerModelTransform::print(bool verbose)
{
// 	Lock lock(mutex);
// 	printf("Transform: %s\n", qPrintable(id));
// 	if (verbose)
// 	{
// 		((QMat *)this)->print(qPrintable(id));
// 		getTr().print(id+"_T");
// 		//extractAnglesR().print(id+"_R");
// 	}
}

void InnerModelTransform::save(QTextStream &out, int tabs)
{
// 	Lock lock(mutex);
// 	QList<InnerModelNode*>::iterator c;
// 
// 	if (id == "root")
// 	{
// 		for (int i=0; i<tabs; i++) out << "\t";
// 		out << "<innermodel>\n";
// 		for (c = children->begin(); c != children->end(); c++) (*c)->save(out, tabs+1);
// 		for (int i=0; i<tabs; i++) out << "\t";
// 		out << "</innermodel>\n";
// 	}
// 	else
// 	{
// 		for (int i=0; i<tabs; i++) out << "\t";
// 		if (gui_translation and not gui_rotation)
// 			out << "<translation id=\"" << id << "\" tx=\""<< QString::number(backtX, 'g', 10) <<"\" ty=\""<< QString::number(backtY, 'g', 10) <<"\" tz=\""<< QString::number(backtZ, 'g', 10) <<"\">\n";
// 		else if (gui_rotation and not gui_translation)
// 			out << "<rotation id=\"" << id << "\" rx=\""<< QString::number(backrX, 'g', 10) <<"\" ry=\""<< QString::number(backrY, 'g', 10) <<"\" rz=\""<< QString::number(backrZ, 'g', 10) <<"\">\n";
// 		else
// 			out << "<transform id=\"" << id << "\" tx=\""<< QString::number(backtX, 'g', 10) <<"\" ty=\""<< QString::number(backtY, 'g', 10) <<"\" tz=\""<< QString::number(backtZ, 'g', 10) <<"\"  rx=\""<< QString::number(backrX, 'g', 10) <<"\" ry=\""<< QString::number(backrY, 'g', 10) <<"\" rz=\""<< QString::number(backrZ, 'g', 10) <<"\">\n";
// 
// 		for (c=children->begin(); c!=children->end(); c++)
// 			(*c)->save(out, tabs+1);
// 
// 		for (int i=0; i<tabs; i++) out << "\t";
// 		if (gui_translation and not gui_rotation )
// 			out << "</translation>\n";
// 		else if (gui_rotation and not gui_translation)
// 			out << "</rotation>\n";
// 		else
// 			out << "</transform>\n";
// 	}
}

/**
 * @brief Updates the internal values of the node from the values passed in the parameters
 *
 * @param tx_ X Translation
 * @param ty_ Y Translation
 * @param tz_ Z Translation
 * @param rx_ RX Rotation
 * @param ry_ RY Rotation
 * @param rz_ RZ Rotation
 * @return void
 */
void InnerModelTransform::update(float tx_, float ty_, float tz_, float rx_, float ry_, float rz_)
{
	Lock lock(mutex);
	backrX = rx_; backrY = ry_; backrZ = rz_;
	backtX = tx_; backtY = ty_; backtZ = tz_;
	set(backrX, backrY, backrZ, backtX, backtY, backtZ);
	fixed = true;
}

void InnerModelTransform::updateT(float tx_, float ty_, float tz_)
{
	Lock lock(mutex);
	backtX = tx_; backtY = ty_; backtZ = tz_;
	set(backrX, backrY, backrZ, backtX, backtY, backtZ);
	fixed = true;
}
	
void InnerModelTransform::updateR(float rx_, float ry_, float rz_)
{
	Lock lock(mutex);
	backrX = rx_; backrY = ry_; backrZ = rz_;
	set(backrX, backrY, backrZ, backtX, backtY, backtZ);
	fixed = true;
}	

InnerModelNode::NodePtr InnerModelTransform::copyNode(THash hash, NodePtr parent)
{
	Lock lock(mutex);
	TransformPtr ret = std::make_shared( new InnerModelTransform(id, engine, backtX, backtY, backtZ, backrX, backrY, backrZ, mass, parent));
	ret->level = level;
	ret->fixed = fixed;
	ret->children->clear();
	ret->attributes.clear();
// 	hash->insert(id,ret);
// 
// 	for (QList<InnerModelNode*>::iterator i=children->begin(); i!=children->end(); i++)
// 	{
// 		ret->addChild((*i)->copyNode(hash, ret));
// 	}

	return ret;
}

void InnerModelTransform::transformValues(const RTMat &Tpb, float tx, float ty, float tz, float rx, float ry, float rz, const NodePtr parentNode)
{	
	RTMat Tbi;
	Tbi.setTr(tx,ty,tz);
	Tbi.setR (rx,ry,rz);
	RTMat Tpi = Tpb*Tbi;
	QVec angles = Tpi.extractAnglesR();
	QVec tr = Tpi.getTr();

	update(angles.x(),angles.y(),angles.z(),tr.x(),tr.y(),tr.z());
}

void InnerModelTransform::translateValues(const RTMat &Tpb, float tx, float ty, float tz, const NodePtr parentNode)
{
	RTMat Tbi;
	Tbi.setTr(tx,ty,tz);
	RTMat Tpi = Tpb*Tbi;
	QVec tr = Tpi.getTr();
	tx = tr.x();
	ty = tr.y();
	tz = tr.z();
	updateT(tx,ty,tz);
}

void InnerModelTransform::rotateValues(const RTMat &Tpb, float rx, float ry, float rz, const NodePtr parentNode)
{
	RTMat Tbi;
	Tbi.setR (rx,ry,rz);
	RTMat Tpi = Tpb*Tbi;
	QVec angles = Tpi.extractAnglesR();
	rx = angles.x();
	ry = angles.y();
	rz = angles.z();
	updateR(rx,ry,rz);
}
