[@bs.module "@reach/dialog"]
external make: React.component(Js.t({..})) = "Dialog";
[@react.component]
let make =
    (
      ~isOpen=true,
      ~allowPinchZoom=true,
      ~onDismiss: unit => unit,
      ~className="",
      ~ariaLabel: string,
      ~children: React.element,
    ) =>
  React.createElement(
    make,
    {
      "isOpen": isOpen,
      "onDismiss": onDismiss,
      "allowPinchZoom": allowPinchZoom,
      "aria-label": ariaLabel,
      "className": className,
      "children": children,
    },
  );
