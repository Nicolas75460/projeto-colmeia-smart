"use client";
import React, { ReactNode } from "react";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "../../ui/dialog";
import { Form } from "../../ui/form";
import { FormProvider, useFormContext } from "react-hook-form";

interface DeviceDialogProps {
  trigger: ReactNode;
  title: string;
  description?: string;
  onSubmit?: (e?: React.BaseSyntheticEvent) => Promise<void>;
  children: ReactNode;
  open: boolean;
  onOpenChange: (open: boolean) => void;
  maxWidth?: string;
}

export const DeviceDialog = ({
  trigger,
  title,
  description,
  onSubmit,
  children,
  open,
  onOpenChange,
  maxWidth = "max-w-md"
}: DeviceDialogProps) => {
  const form = useFormContext();

  return (
    <FormProvider {...form}>
      <Dialog open={open} onOpenChange={onOpenChange}>
        <DialogTrigger asChild>
          {trigger}
        </DialogTrigger>

        <DialogContent className={maxWidth}>
          <DialogHeader>
            <DialogTitle>{title}</DialogTitle>
            <DialogDescription className="py-4">
              {description}
            </DialogDescription>
          </DialogHeader>

          <Form {...form}>
            <form onSubmit={onSubmit} className="space-y-6 text-muted-foreground">
              {children}
            </form>
          </Form>
        </DialogContent>
      </Dialog>
    </FormProvider>
  );
};