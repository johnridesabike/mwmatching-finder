/*
 type value;
 module Drag = {
   type t = {
     x: value,
     y: value,
     mutable fx: Js.Nullable.t(value),
     mutable fy: Js.Nullable.t(value),
   };

   [@bs.send]
   external on:
     (
       t,
       [@bs.string] [ | `start | `drag | [@bs.as "end"] `end_],
       [@bs.uncurry] (t => unit)
     ) =>
     t =
     "on";
   [@bs.module "d3-drag"] external make: unit => t = "drag";
 };
 */

module Force = {
  module Node = {
    /**
     * Any arbitrary data you wish to add will be namespaced to the `data`
     * field.
     */
    type t('node) = {
      index: int,
      x: float,
      y: float,
      vx: float,
      vy: float,
      data: 'node,
    };
  };

  /**
   * The simulation mutates the node objects, so this copies the data into an
   * opaque type.
   */
  module MakeNode = {
    type t('node);
    [@bs.obj] external make: (~data: 'node) => t('node);
  };

  module Link = {
    /**
     * Any arbitrary data you wish to add will be namespaced to the `data`
     * field.
     */
    type t('node, 'link) = {
      source: Node.t('node),
      target: Node.t('node),
      data: 'link,
    };
  };

  module MakeLink = {
    type t('node, 'link);
    [@bs.obj]
    external make:
      (~source: string, ~target: string, ~data: 'link) => t('node, 'link);
  };

  module Links = {
    type t('node, 'link);
    [@bs.module "d3-force"]
    external make: array(MakeLink.t('node, 'link)) => t('node, 'link) =
      "forceLink";
    [@bs.send]
    external id:
      (t('node, 'link), [@bs.uncurry] (Node.t('node) => string)) =>
      t('node, 'link) =
      "id";
    [@bs.send]
    external distance:
      (t('node, 'link), [@bs.uncurry] (Node.t('node) => int)) =>
      t('node, 'link) =
      "distance";
    /**
     * Only use this after adding the links to a simulation. The simulation
     * mutates the data by replacing `source` and `target` with references to
     * the objects with the ids of the original strings.
     */
    [@bs.send]
    external unsafeToArray: t('node, 'link) => array(Link.t('node, 'link)) =
      "links";
  };

  module ManyBody = {
    type t;
    [@bs.module "d3-force"] external make: unit => t = "forceManyBody";
    [@bs.send]
    external strength: (t, [@bs.uncurry] (unit => float)) => t = "strength";
    [@bs.send] external distanceMin: (t, float) => t = "distanceMin";
    [@bs.send] external distanceMax: (t, float) => t = "distanceMax";
  };
  module Radial = {
    type t;
    [@bs.module "d3-force"] external make: float => t = "forceRadial";
  };
  module Center = {
    type t;
    [@bs.module "d3-force"]
    external make: (~width: int, ~height: int) => t = "forceCenter";
  };
  module Simulation = {
    type t('node, 'link);
    [@bs.module "d3-force"]
    external make: array(MakeNode.t('node)) => t('node, 'link) =
      "forceSimulation";
    [@bs.send]
    external setForceLink:
      (t('node, 'link), [@bs.as "link"] _, Links.t('node, 'link)) =>
      t('node, 'link) =
      "force";
    [@bs.send]
    external setForceChargeManyBody:
      (t('node, 'link), [@bs.as "charge"] _, ManyBody.t) => t('node, 'link) =
      "force";
    [@bs.send]
    external setForceChargeRadial:
      (t('node, 'link), [@bs.as "charge"] _, Radial.t) => t('node, 'link) =
      "force";
    [@bs.send]
    external setForceCenter:
      (t('node, 'link), [@bs.as "center"] _, Center.t) => t('node, 'link) =
      "force";
    [@bs.send]
    external on:
      (
        t('node, 'link),
        [@bs.string] [ | `tick],
        [@bs.uncurry] (unit => unit)
      ) =>
      t('node, 'link) =
      "on";
    [@bs.send]
    external tick: (t('node, 'link), int) => t('node, 'link) = "tick";

    [@bs.send]
    external alphaTarget: (t('node, 'link), float) => t('node, 'link) =
      "alphaTarget";
    [@bs.send] external restart: t('node, 'link) => unit = "restart";

    [@bs.send]
    external getNodes: t('node, 'link) => array(Node.t('node)) = "nodes";
  };
};
/*
 module Selection = {
   module Update = {
     type t;
     [@bs.send] external append: (t, string) => t = "append";
     [@bs.send] external remove: (t, string) => t = "remove";
   };
   type t;
   [@bs.module "d3-selection"] external select: Dom.element => t = "select";
   [@bs.send] external selectAll: (t, string) => t = "selectAll";
   [@bs.send] external attrArray: (t, string, array('a)) => t = "attr";
   [@bs.send] external attr: (t, string, 'a) => t = "attr";

   [@bs.send] external append: (t, string) => t = "append";
   [@bs.send] external data: (t, 'a) => t = "data";
   [@bs.send]
   external join: (t, ~enter: [@bs.uncurry] (Update.t => Update.t), unit) => t =
     "join";
   [@bs.send] external call: (t, Drag.t) => t = "call";
   [@bs.send]
   external text: (t, [@bs.uncurry] (Force.Node.t('a) => 'b)) => t = "text";
   [@bs.send] external transition: t => unit = "transition";
   [@bs.send] external remove: t => t = "remove";

   module Event = {
     type t = {
       x: value,
       y: value,
       active: bool,
     };
   };
   [@bs.module "d3-selection"] external event: Event.t = "event";
 };

 module Shape = {
   type t;
   [@bs.module "d3-shape"] external line: unit => t = "line";
 };
 */
