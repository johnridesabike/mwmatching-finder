type value;

module Event = {
  type t = {
    x: value,
    y: value,
    active: bool,
  };
};

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
      (. t) => unit
    ) =>
    t =
    "on";
  [@bs.module "d3"] external make: unit => t = "drag";
};

module ForceSimulation = {
  module Node = {
    type t('a) = {
      index: int,
      x: float,
      y: float,
      vx: float,
      vy: float,
      data: 'a,
    };
    [@bs.obj] external make: (~data: 'a) => t('a);
  };

  module Links = {
    type t;
    [@bs.module "d3"] external make: array(Js.t({..})) => t = "forceLink";
    [@bs.send] external id: (t, (. Node.t('a)) => 'a) => t = "id";
    [@bs.get] external source: t => Node.t('a) = "source";
    [@bs.get] external target: t => Node.t('a) = "target";
  };

  module ManyBody = {
    type t;
    [@bs.module "d3"] external make: unit => t = "forceManyBody";
  };

  module Center = {
    type t;
    [@bs.module "d3"] external make: unit => t = "forceCenter";
  };

  type t('a);
  [@bs.module "d3"]
  external make: array(Node.t('a)) => t('a) = "forceSimulation";
  [@bs.send]
  external forceLink: (t('a), [@bs.as "link"] _, Links.t) => t('a) =
    "force";
  [@bs.send]
  external forceCharge: (t('a), [@bs.as "charge"] _, ManyBody.t) => t('a) =
    "force";
  [@bs.send]
  external forceCenter: (t('a), [@bs.as "center"] _, Center.t) => t('a) =
    "force";
  [@bs.send]
  external on: (t('a), [@bs.string] [ | `tick], (. unit) => unit) => t('a) =
    "on";

  [@bs.send] external alphaTarget: (t('a), float) => t('a) = "alphaTarget";
  [@bs.send] external restart: t('a) => unit = "restart";
};

module Selection = {
  type t;
  [@bs.module "d3"] external select: 'a => t = "select";
  [@bs.send] external selectAll: (t, string) => t = "selectAll";
  [@bs.send]
  external attrArray: (t, [@bs.string] [ | `viewBox], array('a)) => t =
    "attr";
  [@bs.send]
  external attr:
    (
      t,
      [@bs.string] [
        | `stroke
        | [@bs.as "stroke-opacity"] `strokeOpacity
        | [@bs.as "stroke-width"] `strokeWidth
        | `r
        | `x1
        | `x2
        | `y1
        | `y2
        | `cx
        | `cy
      ],
      'a
    ) =>
    t =
    "attr";

  [@bs.send] external append: (t, string) => t = "append";
  [@bs.send] external data: (t, 'a) => t = "data";
  [@bs.send] external join: (t, string) => t = "join";
  [@bs.send] external call: (t, Drag.t) => t = "call";
  [@bs.send]
  external text: (t, (. ForceSimulation.Node.t('a)) => 'b) => t = "text";
};

type t = {event: Event.t};
[@bs.module] external d3: t = "d3";
