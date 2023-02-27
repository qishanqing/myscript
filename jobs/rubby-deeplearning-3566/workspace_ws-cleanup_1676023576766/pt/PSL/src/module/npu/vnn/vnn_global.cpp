//
// Created by hao on 2021/6/9.
//
#include "vnn_global.h"

void vnn_Release(vsi_nn_graph_t *graph, vsi_bool release_ctx)
{
    vsi_nn_context_t ctx;
    if (NULL != graph)
    {
        ctx = graph->ctx;
        vsi_nn_ReleaseGraph(&graph);

        /*-----------------------------------------
        Unregister client ops
        -----------------------------------------*/


        if (release_ctx)
        {
            vsi_nn_ReleaseContext(&ctx);
        }
    }
}