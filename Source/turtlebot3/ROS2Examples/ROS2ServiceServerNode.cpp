// Copyright 2020-2021 Rapyuta Robotics Co., Ltd.

#include "ROS2ServiceServerNode.h"

AROS2ServiceServerNode::AROS2ServiceServerNode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AROS2ServiceServerNode::BeginPlay()
{
    Super::BeginPlay();
    Init();

    // bound callback function
    FServiceCallback AddTwoIntsSrvCallback;
    AddTwoIntsSrvCallback.BindDynamic(this, &AROS2ServiceServerNode::SrvCallback);

    // Add serivce server to ROS2Node
    AddServiceServer(ServiceName, UROS2AddTwoIntsSrv::StaticClass(), AddTwoIntsSrvCallback);
}

void AROS2ServiceServerNode::SrvCallback(UROS2GenericSrv* InService)
{
    UROS2AddTwoIntsSrv* AddTwoIntsService = Cast<UROS2AddTwoIntsSrv>(InService);

    FROSAddTwoIntsReq req;
    AddTwoIntsService->GetRequest(req);

    // Add two ints.
    FROSAddTwoIntsRes res;
    res.Sum = req.A + req.B;

    // Set response.
    AddTwoIntsService->SetResponse(res);

    // Log request and response
    UE_LOG(
        LogTurtlebot3, Log, TEXT("[%s][%s][C++][receive request] %d + %d = %d"), *GetName(), *ServiceName, req.A, req.B, res.Sum);
}