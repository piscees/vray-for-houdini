//
// Copyright (c) 2015-2016, Chaos Software Ltd
//
// V-Ray For Houdini
//
// ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
//
// Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
//

#ifndef VRAY_FOR_HOUDINI_EXPORT_PRIMITIVE_H
#define VRAY_FOR_HOUDINI_EXPORT_PRIMITIVE_H

#include "vfh_vray.h"
#include "vfh_exporter.h"

#include <OBJ/OBJ_Geometry.h>
#include <GU/GU_Detail.h>

namespace VRayForHoudini {

typedef std::vector< VRay::Plugin > PluginList;
typedef std::list< Attrs::PluginDesc > PluginDescList;

/// Base class for exporting primitives from OBJ nodes
class PrimitiveExporter {
public:
	PrimitiveExporter(OBJ_Node &obj, OP_Context &ctx, VRayExporter &exp): m_object(obj), m_context(ctx), m_exporter(exp) {}

	/// TODO: is this needed?
	virtual bool asPluginDesc(const GU_Detail &gdp, Attrs::PluginDesc &pluginDesc) = 0;

	/// Generate plugin descriptions for all supported primitives in the provided GU_Detail
	/// @gdp - the detail to traverse
	/// @plugins[out] - the list of plugins generated for this detail
	virtual void exportPrimitives(const GU_Detail &detail, PluginDescList &plugins) = 0;
	virtual ~PrimitiveExporter() {}
protected:
	OBJ_Node     &m_object; ///< Object node owner of all details that will be passed to exportPrimitives
	OP_Context   &m_context; ///< Current context used to obtain current time
	VRayExporter &m_exporter; ///< Exporter instance for writing plugins
};

typedef std::shared_ptr<PrimitiveExporter> PrimitiveExporterPtr;

#ifdef CGR_HAS_AUR
/// Exports all VRayVolumeGridRef primitives of passed details
class VolumeExporter: public PrimitiveExporter {
public:
	VolumeExporter(OBJ_Node &obj, OP_Context &ctx, VRayExporter &exp): PrimitiveExporter(obj, ctx, exp) {};

	virtual bool asPluginDesc(const GU_Detail &gdp, Attrs::PluginDesc &pluginDesc) VRAY_OVERRIDE { return false; }
	/// Generate plugin descriptions for all supported primitives in the provided GU_Detail
	/// @gdp - the detail to traverse
	/// @plugins[out] - the list of plugins generated for this detail
	virtual void exportPrimitives(const GU_Detail &detail, PluginDescList &plugins) VRAY_OVERRIDE;

protected:
	/// Export the PhxShaderCache for the given primitive
	/// @prim - the primitive
	void exportCache(const GA_Primitive &prim);

	/// Export the PhxShaderSim for the given SHOP node and associate it with a PhxShaderCache by name
	/// @shop - pointer to the SHOP node containing the sim properties
	/// @overrideAttrs - list of attributes that need to be overriden in the sim plugin, e.g. node_transform
	/// @cacheName - the name of the PhxShaderCache for the 'cache' property
	void exportSim(SHOP_Node *shop, const Attrs::PluginAttrs &overrideAttrs, const std::string &cacheName);
};

/// Specialization for exporting Houdini's volumes as textures
/// Uses VolumeExporter::exportCache and VolumeExporter::exportSim to export needed plugins
class HoudiniVolumeExporter: public VolumeExporter {
public:
	HoudiniVolumeExporter(OBJ_Node &obj, OP_Context &ctx, VRayExporter &exp): VolumeExporter(obj, ctx, exp) {};

	/// Generate plugin descriptions for all supported primitives in the provided GU_Detail
	/// @gdp - the detail to traverse
	/// @plugins[out] - the list of plugins generated for this detail
	virtual void exportPrimitives(const GU_Detail &detail, PluginDescList &plugins) VRAY_OVERRIDE;
private:

};
#endif // CGR_HAS_AUR

} // namespace VRayForHoudini

#endif // VRAY_FOR_HOUDINI_EXPORT_PRIMITIVE_H



