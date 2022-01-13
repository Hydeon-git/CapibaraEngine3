#pragma once

//#include "..\..\..\CapibaraEngine3\Engine\Source\GameObject.h"
///*-----------------COMPONENTS-------------------*/
////#include "..\..\..\CapibaraEngine3\Engine\Source\ComponentScript.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\TransformComponent.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\CameraComponent.h"
////#include "..\..\..\CapibaraEngine3\Engine\Source\LightComponent.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\MaterialComponent.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\MeshComponent.h"
///*-----------------COMPONENTS-------------------*/
//
///*-----------------MATH-------------------*/
//#include "..\..\..\CapibaraEngine3\Dependencies\MathGeoLib\src\Math\float3.h"
//#include "..\..\..\CapibaraEngine3\Dependencies\MathGeoLib\src\Math\Quat.h"
//#include "..\..\..\CapibaraEngine3\Dependencies\MathGeoLib\src\Math\float4x4.h"
///*-----------------MATH-------------------*/
//
///*-----------------HELPERS-------------------*/
//
//#include "..\..\..\CapibaraEngine3\Engine\Source\Color.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\StaticInput.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Maths.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Debug.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\RandomHelper.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Time.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Camera.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Gizmos.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Prefab.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\SceneManager.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\AlienEngine.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\RayCreator.h"
//#include "..\..\..\CapibaraEngine3\Engine\Source\Screen.h"

/*-----------------HELPERS-------------------*/

#ifdef CAPIBARAENGINESCRIPTS_EXPORTS
#define CAPIBARA_ENGINE_API __declspec(dllexport)
#else
#define CAPIBARA_ENGINE_API __declspec(dllimport)
#endif

// define it next to the CreateClass/StructFunct to be able to use the class/struct
#define CAPIBARA_FACTORY extern "C" CAPIBARA_ENGINE_API

// ------------INSPECTOR MACROS----------------\\
/*--------------------int--------------------*/
#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_, #INT_)
#define SHOW_IN_INSPECTOR_AS_DRAGABLE_INT(INT_) ComponentScript::InspectorDragableInt(&INT_, #INT_)
#define SHOW_IN_INSPECTOR_AS_SLIDER_INT(INT_, MIN_, MAX_) ComponentScript::InspectorSliderInt(&INT_, #INT_, MIN_, MAX_)
/*--------------------float--------------------*/
#define SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(FLOAT_) ComponentScript::InspectorInputFloat(&FLOAT_, #FLOAT_)
#define SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(FLOAT_) ComponentScript::InspectorDragableFloat(&FLOAT_, #FLOAT_)
#define SHOW_IN_INSPECTOR_AS_SLIDER_FLOAT(FLOAT_, MIN_, MAX_) ComponentScript::InspectorSliderFloat(&FLOAT_, #FLOAT_, MIN_, MAX_)
/*--------------------bool--------------------*/
#define SHOW_IN_INSPECTOR_AS_CHECKBOX_BOOL(BOOL_) ComponentScript::InspectorBool(&BOOL_, #BOOL_)
/*--------------------prefab--------------------*/
#define SHOW_IN_INSPECTOR_AS_PREFAB(PREFAB_) ComponentScript::InspectorPrefab(&PREFAB_, #PREFAB_)
/*--------------------gameobject--------------------*/
#define SHOW_IN_INSPECTOR_AS_GAMEOBJECT(GAMEOBJECT_) ComponentScript::InspectorGameObject(&GAMEOBJECT_, #GAMEOBJECT_)
// ------------INSPECTOR MACROS----------------\\

#define VARAIBLE_TO_STRING(VAR_) #VAR_

#define LOG(format, ...) Debug::Log(__FILE__, __LINE__, format, __VA_ARGS__);

