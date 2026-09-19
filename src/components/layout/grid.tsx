import {cva, type VariantProps} from "class-variance-authority";
import {cn} from "cn";
import * as React from "react";

const gridVariants = cva("grid", {
  variants: {
    columns: {
      1: "grid-cols-1",
      2: "grid-cols-2",
      3: "grid-cols-3",
      4: "grid-cols-4",
      5: "grid-cols-5",
      6: "grid-cols-6",
      12: "grid-cols-12",
    },
    rows: {
      1: "grid-rows-1",
      2: "grid-rows-2",
      3: "grid-rows-3",
      4: "grid-rows-4",
      5: "grid-rows-5",
      6: "grid-rows-6",
    },
    gap: {
      0: "gap-0",
      0.5: "gap-0.5",
      1: "gap-1",
      1.5: "gap-1.5",
      2: "gap-2",
      2.5: "gap-2.5",
      3: "gap-3",
      4: "gap-4",
      5: "gap-5",
      6: "gap-6",
      8: "gap-8",
      10: "gap-10",
      12: "gap-12",
    },
  },
});

type GridProps = React.ComponentProps<"div"> &
  VariantProps<typeof gridVariants>;

export function Grid({className, columns, rows, gap, ...props}: GridProps) {
  return (
    <div
      data-slot="grid"
      className={cn(gridVariants({columns, rows, gap}), className)}
      {...props}
    />
  );
}

const gridColumnVariants = cva("", {
  variants: {
    span: {
      1: "col-span-1",
      2: "col-span-2",
      3: "col-span-3",
      4: "col-span-4",
      5: "col-span-5",
      6: "col-span-6",
      7: "col-span-7",
      8: "col-span-8",
      9: "col-span-9",
      10: "col-span-10",
      11: "col-span-11",
      12: "col-span-12",
      full: "col-span-full",
    },
  },
});

type GridColumnProps = React.ComponentProps<"div"> &
  VariantProps<typeof gridColumnVariants>;

export function GridColumn({className, span, ...props}: GridColumnProps) {
  return (
    <div
      data-slot="grid-column"
      className={cn(gridColumnVariants({span}), className)}
      {...props}
    />
  );
}

const gridRowVariants = cva("", {
  variants: {
    span: {
      1: "row-span-1",
      2: "row-span-2",
      3: "row-span-3",
      4: "row-span-4",
      5: "row-span-5",
      6: "row-span-6",
      full: "row-span-full",
    },
  },
});

type GridRowProps = React.ComponentProps<"div"> &
  VariantProps<typeof gridRowVariants>;

export function GridRow({className, span, ...props}: GridRowProps) {
  return (
    <div
      data-slot="grid-row"
      className={cn(gridRowVariants({span}), className)}
      {...props}
    />
  );
}
