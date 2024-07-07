#include "Core/RpaiSubsystem.h"
#include "Core/RpaiPlannerBase.h"

const URpaiPlannerBase* URpaiSubsystem::DuplicateOrGetPlannerInstanceInWorldScope(const URpaiPlannerBase* Template)
{
    if (!IsValid(Template))
    {
        return nullptr;
    }
    else if (GetWorld()->IsPlayInEditor())
    {
        auto Found = InstantiatedTemplates.Find(Template);
        if (Found != nullptr)
        {
            return *Found;
        }
        else
        {
            auto Instance = InstantiatedTemplates.Add(Template, Cast<URpaiPlannerBase>(StaticDuplicateObject(Template, this)));
            return Instance;
        }
    }
    else if(Template->GetWorld() != GetWorld())
    {
        auto Found = InstantiatedTemplates.Find(Template);
        if(Found != nullptr)
        {
            return *Found;
        }
        else
        {
            auto Instance = InstantiatedTemplates.Add(Template, Cast<URpaiPlannerBase>(StaticDuplicateObject(Template, this)));
            return Instance;
        }
    }
    else if (Template->HasAnyFlags(RF_ClassDefaultObject))
    {
        auto Found = InstantiatedTemplates.Find(Template);
        if(Found != nullptr)
        {
            return *Found;
        }
        else
        {
            auto Instance = InstantiatedTemplates.Add(Template, Cast<URpaiPlannerBase>(StaticDuplicateObject(Template, this)));
            return Instance;
        }
    }
    else
    {
        return Template;
    }
}
